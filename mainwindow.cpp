#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDebug>
#include <QModelIndexList>
#include <algorithm>

#include "DivXlogic/subtitleio.h"
#include "edittitledialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUI(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::discardChangesDialog() const
{
    QMessageBox msgBox;
    msgBox.setText("There is a subtitle loaded.");
    msgBox.setInformativeText("Do you want to save the current state?");
    msgBox.setStandardButtons(QMessageBox::Save |
                              QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

void MainWindow::refreshTitleList()
{
    // reset table with subtitles
    delete ui->tableView->model();
    QStandardItemModel *model =
            new QStandardItemModel(0, 3);
    QStringList horizontalLabels =
        {"Start", "End", "Subtitle text"};
    model->setHorizontalHeaderLabels(horizontalLabels);

    long ind = 0;
    QList<QStandardItem*> list;
    for (Subtitle* &row : subtitleApp.getSubtitles().getTitles())
    {
        // add title to table
        if (searchPhrase.length() > 0 &&
                !row->getText().contains(searchPhrase)) continue;
        list.clear();
        list.append(new QStandardItem(row->getSStart()));
        list.append(new QStandardItem(row->getSEnd()));
        list.append(new QStandardItem(row->getText()));
        model->appendRow(list);
        ind++;
    }
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->
            setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setColumnWidth(0, 75);
    ui->tableView->setColumnWidth(1, 75);
    ui->tableView->setColumnWidth(2, ui->tableView->width() - 200);
    ui->tableView->horizontalHeader()->
            setSectionResizeMode(QHeaderView::Fixed);
}

void MainWindow::actionLoad()
{
    if (subtitleApp.isLoaded())
    {
        switch (discardChangesDialog()) {
        case QMessageBox::Save:
            if (subtitleApp.getFilePath().length() > 0) actionSave();
            else actionSaveAs();
            break;
        case QMessageBox::Cancel: return; break;
        }
    }
    QString path = QFileDialog::getOpenFileName(
                this, "Open file", "",
                "Titles (*.srt *.sub *.txt);;All Files(*)");
    if (path.length() == 0) return;
    FORMATS format = SubtitleIO::detect(path);
    ui->loadSubtitleButton->setText("...");
    try {
        setUI(false);
        subtitleApp.loadTitle(path, format,
                              ui->iFPSDoubleSpinBox->value());
        setUI(true);
        ui->editButton->setEnabled(true);
    } catch (...) {}
    updateWindowTitle();
    ui->loadSubtitleButton->setText("Load Subtitle");
    ui->FPSDoubleSpinBox->setValue(subtitleApp.getSubtitles().getFPS());
    ui->FPSlabel->setText("Current FPS:");
    refreshTitleList();
}

void MainWindow::actionSave()
{
    ui->saveSubtitleButton->setText("...");
    try
    {
        subtitleApp.saveTitle();
    }
    catch (...) { /* show message - umable to save */ }
    ui->saveSubtitleButton->setText("Save Subtitle");
}

void MainWindow::actionSaveAs()
{
    // Get saving format
    FORMATS format;
    QString exts = "";
    if (ui->SRTRadioButton->isChecked())
    {
        format = FORMATS::SRT;
        exts = "*.srt";
    }
    else if (ui->MPSubRadioButton->isChecked())
    {
        format = FORMATS::MPSub;
        exts = "*.sub";
    }
    else if (ui->MicroDVDRadioButton->isChecked())
    {
        format = FORMATS::MicroDVD;
        exts = "*.sub *.txt";
    }
    else { return; /* throw wrong format */ }

    QString path = QFileDialog::getSaveFileName(
                this, "Save file", "",
                "Titles (" + exts + ");;All Files(*)");

    if (path.length() == 0) return;
    ui->saveSubtitleAsButton->setText("...");
    try
    {
        subtitleApp.saveTitle(path, format);
    }
    catch (...) { setUI(false); }
    updateWindowTitle();
    ui->saveSubtitleAsButton->setText("Save Subtitle As");
}

void MainWindow::actionEdit()
{
    QModelIndexList indexes =
            ui->tableView->selectionModel()->
            selection().indexes();
    if (indexes.count() != 3)
    {
        /*  */
        return;
    }
    QString data = ui->tableView->model()->
            data(ui->tableView->model()->
                 index(indexes.at(0).row(), 0)).toString();
    long row = 0;
    for (auto it = subtitleApp.getSubtitles().getTitles().begin();
         it != subtitleApp.getSubtitles().getTitles().end(); it++)
    {
        if ((*it)->getSStart() == data) break;
        row++;
    }

    editTitleDialog dialog(
                subtitleApp.getSubtitles().getTitles()[row],
                this);
    dialog.setModal(true);
    dialog.exec();
    std::sort(subtitleApp.getSubtitles().getTitles().begin(),
              subtitleApp.getSubtitles().getTitles().end(),
              [](const Subtitle *a, const Subtitle *b) -> bool {
        return a->getStart() < b->getStart();
    });
    refreshTitleList();
}

void MainWindow::setUI(bool state)
{
    ui->saveSubtitleButton->setEnabled(state);
    ui->saveSubtitleAsButton->setEnabled(state);
    ui->saveTypeGroupBox->setEnabled(state);
    ui->FPSDoubleSpinBox->setEnabled(state);
    ui->editButton->setEnabled(state);
}

void MainWindow::updateWindowTitle()
{
    if (subtitleApp.isLoaded())
        this->setWindowTitle(PROGRAM_TITLE + " | " +
                             subtitleApp.getFilePath());
    else
        this->setWindowTitle(PROGRAM_TITLE);
}

void MainWindow::on_loadSubtitleButton_clicked()
{
    actionLoad();
}

void MainWindow::on_saveSubtitleButton_clicked()
{
    actionSave();
}

void MainWindow::on_saveSubtitleAsButton_clicked()
{
    actionSaveAs();
}

void MainWindow::on_FPSDoubleSpinBox_valueChanged()
{
    ui->FPSlabel->setText("Current FPS:*");
}

void MainWindow::on_FPSDoubleSpinBox_editingFinished()
{
    ui->FPSlabel->setText("Current FPS:");
    subtitleApp.getSubtitles().setFPS(
                ui->FPSDoubleSpinBox->value());
}

void MainWindow::on_editButton_clicked()
{
    actionEdit();
}

void MainWindow::on_tableView_doubleClicked()
{
    actionEdit();
}

void MainWindow::on_searchLineEdit_editingFinished()
{
    searchPhrase = ui->searchLineEdit->text();
    refreshTitleList();
}
