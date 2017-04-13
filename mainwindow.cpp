#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QModelIndexList>
#include <algorithm>

#include "edittitledialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    subtitleApp.newTitle();
    currentModel = nullptr;
    changeContext(0);
    processor.setSubtitles(currentFile);
    ui->statusBar->showMessage("Velcome to DivX Converter!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::discardChangesDialog() const
{
    QMessageBox msgBox;
    msgBox.setText("Closing opened subtitle.");
    msgBox.setInformativeText("Do you want to save the current state?");
    msgBox.setStandardButtons(QMessageBox::Save |
                              QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

void MainWindow::refreshTitleList()
{
    QStandardItemModel *newModel = nullptr;
    if (currentFile->getTitles().size() > 0)
    {
        newModel = new QStandardItemModel(0, 3);
        QStringList horizontalLabels =
            {"Start", "End", "Subtitle text"};
        newModel->setHorizontalHeaderLabels(horizontalLabels);

        long ind = 0;
        QList<QStandardItem*> list;
        for (Subtitle* &row : currentFile->getTitles())
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
    if (currentModel)
    {
        currentModel->clear();
        delete currentModel;
    }
    currentModel = newModel;

    if (currentModel)
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

void MainWindow::changeContext(int ind)
{
    status("*");
    currentFile = subtitleApp.getSubtitles(ind);
    currFileInd = ind;
    processor.setSubtitles(currentFile);
    ui->FPSDoubleSpinBox->setValue(currentFile->getFPS());
    ui->FPSlabel->setText("Current FPS:");
    ui->tabWidget->setCurrentIndex(ind);
    refreshTitleList();
    updateWindowTitle();
    status("");
}

void MainWindow::actionLoad()
{
    QString path = QFileDialog::getOpenFileName(
                this, "Open file", "",
                "Titles (*.srt *.sub *.txt);;All Files(*)");
    if (path.length() == 0) return;

    status("*");
    FORMATS format = SubtitleIO::detect(path);
    try {
        subtitleApp.loadTitle(path, format,
                              ui->iFPSDoubleSpinBox->value());
        ui->tabWidget->addTab(new QWidget, "naziv");
        changeContext(ui->tabWidget->count() - 1);
        status("File loaded! - " + path);
    }
    catch (...)
    {
        status("Unable to load file! - " + path);
    }
}

void MainWindow::actionSave()
{
    status("*");
    try { subtitleApp.saveTitle(currFileInd); }
    catch (...)
    {
        status("Unable do save file! - " +
               currentFile->getFilePath());
    }
    currentFile->setEdited(false);
    status("File saved! - " +
           currentFile->getFilePath());
}

void MainWindow::actionSaveAs(FORMATS format)
{
    // Get saving format if none is provided
    if (format == FORMATS::UNDEFINED)
    {
        if (ui->SRTRadioButton->isChecked())
            format = FORMATS::SRT;
        else if (ui->MPSubRadioButton->isChecked())
            format = FORMATS::MPSub;
        else if (ui->MicroDVDRadioButton->isChecked())
            format = FORMATS::MicroDVD;
        else {
            status("Undefined format while saving the file");
            return;
        }
    }
    QString exts = "";
    switch (format) {
    case FORMATS::SRT: exts = "*.srt"; break;
    case FORMATS::MPSub: exts = "*.sub"; break;
    case FORMATS::MicroDVD: exts = "*.sub *.txt"; break;
    }

    QString path = QFileDialog::getSaveFileName(
                this, "Save file", "",
                "Titles (" + exts + ");;All Files(*)");
    if (path.length() == 0) return;

    status("*");
    try { subtitleApp.saveTitle(path, format, currFileInd); }
    catch (...) {
        status("Unable do save file! - " +
               currentFile->getFilePath());
    }
    updateWindowTitle();
    currentFile->setEdited(false);
    status("File saved! - " + currentFile->getFilePath());
}

void MainWindow::actionEdit()
{
    // modify to work through processor
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

    editTitleDialog dialog(
                currentFile->getTitles()[
                currentFile->indexOf(data)],
                this);
    dialog.setModal(true);
    dialog.exec();
    std::sort(currentFile->getTitles().begin(),
              currentFile->getTitles().end(),
              [](const Subtitle *a, const Subtitle *b) -> bool {
        return a->getStart() < b->getStart();
    });
    refreshTitleList();
}

void MainWindow::actionClose()
{
    if (currentFile->isEdited())
    {
        switch (discardChangesDialog()) {
        case QMessageBox::Save:
            if (currentFile->getFilePath().length() > 0) actionSave();
            else actionSaveAs();
            break;
        case QMessageBox::Cancel: return; break;
        }
    }
    subtitleApp.closeFile(currFileInd);
    //models.erase(models.begin() + currFileInd);
    ui->tabWidget->removeTab(currFileInd);
    if (currFileInd > 0) changeContext(currFileInd - 1);
    else
    {
        if (ui->tabWidget->count() == 0) subtitleApp.newTitle();
        //models.push_back(nullptr);
        changeContext(0);
    }
    status("File closed!");
}

void MainWindow::updateWindowTitle()
{
    this->setWindowTitle(PROGRAM_TITLE + " | " +
                         currentFile->getFilePath());
}

void MainWindow::status(const QString &message)
{
    ui->statusBar->showMessage(message);
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
    currentFile->setFPS(ui->FPSDoubleSpinBox->value());
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

void MainWindow::on_actionOpen_File_triggered()
{
    actionLoad();
}

void MainWindow::on_actionSaveAsSRT_triggered()
{
    actionSaveAs(FORMATS::SRT);
}

void MainWindow::on_actionSaveAsMPSub_triggered()
{
    actionSaveAs(FORMATS::MPSub);
}

void MainWindow::on_actionSaveAsMicroDVD_triggered()
{
    actionSaveAs(FORMATS::MicroDVD);
}

void MainWindow::on_actionExit_triggered()
{
    if (ui->tabWidget->count() > 1)
    {
        // discard all?
    }
    else if (currentFile->isEdited())
    {
        switch (discardChangesDialog()) {
        case QMessageBox::Save:
            if (currentFile->getFilePath().length() > 0) actionSave();
            else actionSaveAs();
            break;
        case QMessageBox::Cancel: return; break;
        }
    }
    this->close();
}

void MainWindow::on_actionClose_File_triggered()
{
    actionClose();
}

void MainWindow::on_actionEdit_Subtitle_triggered()
{
    actionEdit();
}

void MainWindow::on_actionSave_triggered()
{
    actionSave();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    changeContext(index);
}

void MainWindow::on_actionNew_File_triggered()
{
    subtitleApp.newTitle();
}
