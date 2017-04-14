#include "tabform.h"
#include "ui_tabform.h"

#include <QStandardItemModel>
#include <QDebug>

#include "mainwindow.h"

TabForm::TabForm(Subtitles *subs, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabForm),
    file(subs),
    processor(subs, parent)
{
    ui->setupUi(this);
    refreshTitleList();
    updateTitle();
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
    QRegExp rx("[/\\\\](.*)$");
    rx.indexIn(file->getFilePath());
    QString title = rx.cap(1);
    if (file->isEdited()) title += "*";
    setWindowTitle(title);
}

void TabForm::refreshTitleList()
{
    QStandardItemModel *newModel = nullptr;
    if (file->getTitles().size() > 0)
    {
        newModel = new QStandardItemModel(0, 3);
        QStringList horizontalLabels =
            {"Start", "End", "Subtitle text"};
        newModel->setHorizontalHeaderLabels(horizontalLabels);

        long ind = 0;
        QList<QStandardItem*> list;
        for (Subtitle* &row : file->getTitles())
        {
            // add title to table
            if (searchPhrase.length() > 0 &&
                    !row->getText().contains(searchPhrase)) continue;
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
        ui->tableView->setColumnWidth(0, 75);
        ui->tableView->setColumnWidth(1, 75);
        ui->tableView->setColumnWidth(2, ui->tableView->width() - 200);
        ui->tableView->horizontalHeader()->
                setSectionResizeMode(QHeaderView::Fixed);
    }
}

void TabForm::on_tableView_doubleClicked(const QModelIndex &index)
{
    qInfo() << "Double click";
    static_cast<MainWindow*>(parent())->actionEdit();
}
