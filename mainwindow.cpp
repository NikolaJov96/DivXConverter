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
    // new blank file
    subtitleApp.newTitle();
    // new tab for the file
    ui->tabWidget->addTab(new TabForm(subtitleApp.getSubtitles(0)), "new");
    // romove initial tab
    ui->tabWidget->removeTab(0);
    currTab = static_cast<TabForm*> (ui->tabWidget->currentWidget());
    changeContext(0);
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

void MainWindow::changeContext(int ind)
{
    status("*");
    // setup new mainwindow params
    currFileInd = ind;
    // switch tab to requested
    ui->tabWidget->setCurrentIndex(ind);
    currTab = static_cast<TabForm*> (ui->tabWidget->currentWidget());
    currentFile = currTab->getFile();
    processor.setSubtitles(currentFile);

    // update GUI
    ui->FPSDoubleSpinBox->setValue(currentFile->getFPS());
    ui->searchLineEdit->setText(currTab->getSearchPhrase());
    ui->FPSlabel->setText("Current FPS:");
    updateWindowTitle();
    status("");
}

void MainWindow::actionNew()
{
    subtitleApp.newTitle();
    int ind = subtitleApp.getFilesCo() - 1;
    ui->tabWidget->addTab(new TabForm(subtitleApp.getSubtitles(ind)), "new");
    changeContext(ind);
}

void MainWindow::actionLoad()
{
    // Get loading path
    QString path = QFileDialog::getOpenFileName(
                this, "Open file", "",
                "Titles (*.srt *.sub *.txt);;All Files(*)");\
    // Retiurn if loading is canceled
    if (path.length() == 0) return;

    // Load form path
    status("*");
    FORMATS format = SubtitleIO::detect(path);
    try {
        subtitleApp.loadTitle(path, format,
                              ui->iFPSDoubleSpinBox->value());
    }
    catch (...)
    {
        status("Unable to load file! - " + path);
        return;
    }
    int ind = subtitleApp.getFilesCo() - 1;
    ui->tabWidget->addTab(new TabForm(subtitleApp.getSubtitles(ind)), "new");
    changeContext(ind);
    status("File loaded! - " + path);
}

void MainWindow::actionSave()
{
    status("*");
    try { subtitleApp.saveTitle(currFileInd); }
    catch (...)
    {
        status("Unable do save file! - " +
               currentFile->getFilePath());
        return;
    }
    currentFile->setEdited(false);
    updateWindowTitle();
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

    // Chose file extensions
    QString exts = "";
    switch (format)
    {
    case FORMATS::SRT: exts = "*.srt"; break;
    case FORMATS::MPSub: exts = "*.sub"; break;
    case FORMATS::MicroDVD: exts = "*.sub *.txt"; break;
    }

    // Get saving path
    QString path = QFileDialog::getSaveFileName(
                this, "Save file", "",
                "Titles (" + exts + ");;All Files(*)");
    // Return if saving is canceled
    if (path.length() == 0) return;

    // Save to new file and change context
    status("*");
    try { subtitleApp.saveTitle(path, format, currFileInd); }
    catch (...) {
        status("Unable do save file! - " +
               currentFile->getFilePath());
    }
    updateWindowTitle();
    status("File saved! - " + currentFile->getFilePath());
}

// edit is responsibility of subtitle proccessor
void MainWindow::actionEdit()
{
    // modify to work through processor
    QModelIndexList indexes =
            currTab->getTable()->selectionModel()->
            selection().indexes();
    if (indexes.count() != 3)
    {
        /*  */
        return;
    }
    QString data = currTab->getTable()->model()->
            data(currTab->getTable()->model()->
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
    currTab->refreshTitleList();
}

void MainWindow::actionClose()
{
    // Confirm closing
    if (currentFile->isEdited())
    {
        switch (discardChangesDialog())
        {
        case QMessageBox::Save:
            if (currentFile->getFilePath().length() > 0) actionSave();
            else actionSaveAs();
            break;
        case QMessageBox::Cancel: return; break;
        }
    }

    int closeInd = currFileInd;
    // If last tab is going to be closed, prepare new blank tab
    if (subtitleApp.getFilesCo() == 1) actionNew();

    subtitleApp.closeFile(closeInd);
    ui->tabWidget->removeTab(closeInd);
    status("File closed!");

    // Always jump to the first tab
    changeContext(0);
}

void MainWindow::updateWindowTitle()
{
    QString title = PROGRAM_TITLE + " | " +
                currentFile->getFilePath();
    if (currentFile->isEdited()) title += " *";
    this->setWindowTitle(title);
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

// edit is responsibility of subtitle proccessor
void MainWindow::on_FPSDoubleSpinBox_editingFinished()
{
    ui->FPSlabel->setText("Current FPS:");
    currentFile->setFPS(ui->FPSDoubleSpinBox->value());
    updateWindowTitle();
}

void MainWindow::on_editButton_clicked()
{
    actionEdit();
}

void MainWindow::on_searchLineEdit_editingFinished()
{
    currTab->setSearhPhrase(ui->searchLineEdit->text());
    currTab->refreshTitleList();
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
    // change only if different tab is selected
    if (index != currFileInd)
        changeContext(index);
}

void MainWindow::on_actionNew_File_triggered()
{
    actionNew();
}
