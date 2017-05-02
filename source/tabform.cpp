#include "tabform.h"
#include "ui_tabform.h"

#include <QStandardItemModel>
#include <QDebug>

#include "ui_mainwindow.h"

TabForm::TabForm(Subtitles *subs, MainWindow *parent) :
    QWidget(parent),
    mainWindow(parent),
    ui(new Ui::TabForm),
    file(subs),
    processor(subs, parent)
{
    ui->setupUi(this);
    refreshTitleList();
}

TabForm::~TabForm()
{
    if (model)
    {
        model->clear();
        delete model;
    }
    delete ui;
}

QString const &TabForm::getSearchPhrase() const
{
    return searchPhrase;
}

Subtitles *TabForm::getFile() { return file; }

QTableView *TabForm::getTable() { return ui->tableView; }

SubtitleProcessing *TabForm::getProcessor()
{
    return &processor;
}

void TabForm::setSearhPhrase(const QString &phrase)
{
    searchPhrase = phrase;
}

void TabForm::setFile(Subtitles *subs) { file = subs; }

void TabForm::updateTitle()
{
    QString title = "new";
    if (file->getFilePath().length() != 0)
    {
        QRegExp fileNameRX("[/\\\\]([^/\\\\]*)$");
        fileNameRX.indexIn(file->getFilePath());
        title = fileNameRX.cap(1);
    }
    if (file->isEdited()) title += "*";
    mainWindow->ui->tabWidget->setTabText(
                mainWindow->currFileInd, title);
}

void TabForm::refreshTitleList()
{
    QStandardItemModel *newModel = nullptr;
    if (file->getTitles().size() > 0)
    {
        newModel = new QStandardItemModel(0, 3);
        QStringList horizontalLabels =
            {"Start", "End", "Subtitle Text"};
        newModel->setHorizontalHeaderLabels(horizontalLabels);

        long ind = 0;
        QList<QStandardItem*> list;
        for (Subtitle* &row : file->getTitles())
        {
            // add title to table
            if (searchPhrase.length() > 0 &&
                    !row->getText().toLower().contains(searchPhrase.toLower())) continue;
            list.clear();
            list.append(new QStandardItem(row->getSStart()));
            list.append(new QStandardItem(row->getSEnd()));
            list.append(new QStandardItem(row->getText()));
            newModel->appendRow(list);
            ind++;
        }
    }
    ui->tableView->setModel(newModel);
    if (model)
    {
        model->clear();
        delete model;
    }
    model = newModel;

    if (model)
    {
        ui->tableView->verticalHeader()->
                setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        //ui->tableView->setColumnWidth(2, ui->tableView->width() - 200);
        ui->tableView->horizontalHeader()->
                setSectionResizeMode(QHeaderView::Fixed);
    }
}

void TabForm::on_tableView_doubleClicked(const QModelIndex&)
{
    QModelIndexList indexes =
            getTable()->selectionModel()->
            selection().indexes();

    // one row - three items
    if (indexes.count() != 3)
    {
        static_cast<MainWindow*>(parent())->status("Select one table row to edit.");
        return;
    }
    QString data = getTable()->model()->
            data(getTable()->model()->
                 index(indexes.at(0).row(), 0)).toString();

    // refresh GUI only if title is modified
    if (processor.editTitle(data))
    {
        mainWindow->status("*");
        refreshTitleList();
        mainWindow->updateWindowTitle();
        updateTitle();
        mainWindow->updateConsLable();
        mainWindow->status("Modification applied!");
    }
}
