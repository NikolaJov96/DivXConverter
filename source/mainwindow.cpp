#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <algorithm>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QModelIndexList>

const int DEFAULT_MAX_DURATION = 10000;
const int DEFAULT_MAX_DISTANCE = 2000;
const int DEFAULT_MAX_LENGTH = 50;

#include "edittitledialog.h"
#include "timeshiftdialog.h"
#include "autoadjustdialog.h"
#include "splitfiledialog.h"
#include "tabform.h"

MainWindow::MainWindow(int argc, char** argv, QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(710, 490));

    QString message = "";
    if (argc > 1)
    {
        status("*");
        FORMATS format;
        bool fileE = false, contE = false;
        for (int i = 1; i < argc; i++)
        {
            try {
                format = SubtitleIO::detect(argv[i]);
                subtitleApp.loadTitle(argv[i], format,
                                      ui->iFPSDoubleSpinBox->value());
            }
            catch (IOException&) { fileE = true; continue; }
            catch (UndefinedType&) { contE = true; continue; }
            catch (...) { continue; }
            currFileInd = subtitleApp.getFilesCo(); // TabForm::UpdateTitle uses currFileInd;
            ui->tabWidget->addTab(new TabForm(
                                      subtitleApp.getSubtitles(currFileInd - 1), this), "new");
            subtitleApp.getSubtitles(currFileInd - 1)->setConsistency(
                        static_cast<TabForm*>(
                            static_cast<TabForm*>(ui->tabWidget->widget(currFileInd)))->
                        getProcessor()->isConsistent());
            // currFileInd + 1 tab - 0th is the initial blank tab
            static_cast<TabForm*>(ui->tabWidget->widget(currFileInd))->updateTitle();
        }
        if (fileE) message = " - Error while accessing some files!";
        if (contE) message += " - Error while processing smoe files!";
    }
    if (argc == 1 || subtitleApp.getFilesCo() == 0)
    {
        subtitleApp.newTitle();
        ui->tabWidget->addTab(new TabForm(
                                  subtitleApp.getSubtitles(0), this), "new");
    }

    // remove initial tab
    ui->tabWidget->removeTab(0);
    currTab = static_cast<TabForm*> (ui->tabWidget->currentWidget());
    changeContext(0);
    status("Velcome to DivX Converter!" + message);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // If none of the opened files are edited, just close
    bool someEdited = false;
    for (auto &file : subtitleApp.getSubtitles())
        someEdited |= file->isEdited();

    if (!someEdited) event->accept();
    else if (ui->tabWidget->count() > 1)
    {
        switch(QMessageBox::question(this, PROGRAM_TITLE,
                                     tr("More files are open,\ndiscard and close all?"),
                                     QMessageBox::Cancel | QMessageBox::Yes,
                                     QMessageBox::Yes))
        {
        case QMessageBox::Yes: event->accept(); break;
        case QMessageBox::Cancel: event->ignore(); break;
        }
    }
    else if (currentFile->isEdited())
    {
        switch (discardChangesDialog()) {
        case QMessageBox::Save:
            if (currentFile->getFilePath().length() > 0) actionSave();
            else actionSaveAs();
            // merge with Discard case (no break)
        case QMessageBox::Discard:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        }
    }
}

int MainWindow::discardChangesDialog() const
{
    QMessageBox msgBox;
    msgBox.setText("Closing edited subtitle.");
    msgBox.setInformativeText("Do you want to save the current state?");
    msgBox.setStandardButtons(QMessageBox::Save |
                              QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

void MainWindow::changeContext(int ind, bool checkCons)
{
    status("*");
    // setup new mainwindow params
    currFileInd = ind;
    // switch tab to requested
    ui->tabWidget->setCurrentIndex(ind);
    currTab = static_cast<TabForm*> (ui->tabWidget->currentWidget());
    currentFile = currTab->getFile();
    processor = currTab->getProcessor();
    if (checkCons) // skipped by default
        currentFile->setConsistency(processor->isConsistent());

    // update GUI
    ui->FPSDoubleSpinBox->setValue(currentFile->getFPS());
    ui->searchLineEdit->setText(currTab->getSearchPhrase());
    ui->FPSlabel->setText("Current FPS:");
    updateWindowTitle();
    updateConsLable();
    status("");
}

void MainWindow::actionNew()
{
    subtitleApp.newTitle();
    int ind = subtitleApp.getFilesCo() - 1;
    ui->tabWidget->addTab(new TabForm(subtitleApp.getSubtitles(ind), this), "new");
    changeContext(ind);

    // If there is only one opened blank subtitle, close it
    if (subtitleApp.getSubtitles().size() == 2 &&
            subtitleApp.getSubtitles(0)->getTitles().size() == 0)
    {
        subtitleApp.closeFile(0);
        ui->tabWidget->removeTab(0);
    }
}

void MainWindow::actionLoad()
{
    // Get loading path
    QString path = QFileDialog::getOpenFileName(
                this, "Open file", "",
                "Titles (*.srt *.sub *.txt);;All Files(*)");\
    // Retiurn if loading is canceled
    if (path.length() == 0) return;
    for (auto &file : subtitleApp.getSubtitles())
        if (file->getFilePath() == path)
        {
            status("File already opened!");
            return;
        }

    // Load form path
    status("*");
    FORMATS format = SubtitleIO::detect(path);
    try
    {
        subtitleApp.loadTitle(path, format,
                              ui->iFPSDoubleSpinBox->value());
    }
    catch (IOException &e) { status(e.what()); return; }
    catch (UndefinedType &e) { status(e.what()); return; }
    catch (...) { status("Unexpected error."); return; }

    int ind = subtitleApp.getFilesCo() - 1;
    ui->tabWidget->addTab(new TabForm(subtitleApp.getSubtitles(ind), this), "new");
    changeContext(ind, true);
    currTab->updateTitle();


    // If there is only one opened blank subtitle, close it
    if (subtitleApp.getSubtitles().size() == 2 &&
            subtitleApp.getSubtitles(0)->getTitles().size() == 0)
    {
        subtitleApp.closeFile(0);
        ui->tabWidget->removeTab(0);
    }

    status("File loaded! - " + path);
}

void MainWindow::actionSave()
{
    if (currentFile->getTitles().size() == 0)
    {
        status("Why save a blank sibtitle?");
        return;
    }
    // if file is not initially saved requrst Save As first
    if (currentFile->getFilePath().length() == 0)
    {
        status("Save file initialy with Save As!");
        return;
    }

    status("*");
    try { subtitleApp.saveTitle(currFileInd); }
    catch (IOException &e) { status(e.what()); return; }
    catch (UndefinedType &e) { status(e.what()); return; }
    catch (...) { status("Unexpected error."); return; }
    currentFile->setEdited(false);
    updateWindowTitle();
    currTab->updateTitle();
    status("File saved! - " +
           currentFile->getFilePath());
}

void MainWindow::actionSaveAs(FORMATS format)
{
    if (currentFile->getTitles().size() == 0)
    {
        status("Whiy save a blank sibtitle?");
        return;
    }

    // Get saving format if none is provided
    if (format == FORMATS::UNDEFINED)
    {
        status("Undefined format while saving the file");
        return;
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
    catch (IOException &e) { status(e.what()); return; }
    catch (UndefinedType &e) { status(e.what()); return; }
    catch (...) { status("Unexpected error."); return; }

    updateWindowTitle();
    currTab->updateTitle();
    status("File saved! - " + currentFile->getFilePath());
}

void MainWindow::actionEdit()
{
    QModelIndexList indexes =
            currTab->getTable()->selectionModel()->
            selection().indexes();

    // one row - three items
    if (indexes.count() != 3)
    {
        status("Select one table row to edit.");
        return;
    }
    QString data = currTab->getTable()->model()->
            data(currTab->getTable()->model()->
                 index(indexes.at(0).row(), 0)).toString();

    // refresh GUI only if title is modified
    if (processor->editTitle(data))
    {
        status("*");
        currTab->refreshTitleList();
        updateWindowTitle();
        currTab->updateTitle();
        updateConsLable();
        status("Modification applied!");
    }
}

void MainWindow::actionAddSubtitle()
{
    if (processor->addSubtitle())
    {
        status("*");
        currTab->refreshTitleList();
        updateWindowTitle();
        status("Subtitle added!");
    }
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
            else
            {
                status("Save file initialy with Save As!");
                return;
            }
            break;
        case QMessageBox::Cancel: return; break;
        }
    }

    int closeInd = currFileInd;
    // If last tab is going to be closed, prepare new blank tab
    if (subtitleApp.getFilesCo() == 1) actionNew();
    // If actionNew didn't close file, close it
    if (subtitleApp.getFilesCo() > 1)
    {
        subtitleApp.closeFile(closeInd);
        ui->tabWidget->removeTab(closeInd);
    }
    status("File closed!");

    // Always jump to the first tab
    changeContext(0);
}

void MainWindow::actionDelete()
{
    QModelIndexList indexes =
            currTab->getTable()->selectionModel()->
            selection().indexes();

    if (indexes.count() == 0)
    {
        status("Select rows in table to delete.");
        return;
    }

    for (int i = 0; i < indexes.count(); i += 3)
    {
        QString data = currTab->getTable()->model()->
                data(currTab->getTable()->model()->
                     index(indexes.at(i).row(), 0)).toString();
        processor->deleteSubtitle(data);
    }

    // deletion can not fail
    status("*");
    currTab->refreshTitleList();
    updateWindowTitle();
    currTab->updateTitle();
    status("Subtitle(s) deleted!");
}

void MainWindow::actionTimeShift(QModelIndexList *indexes)
{
    if (currentFile->getTitles().size() == 0)
    {
        status("Nothing to shift in the empty table.");
        return;
    }
    if (indexes && indexes->count() == 0)
    {
        status("Select rows in table to shift.");
        return;
    }

    // get delta time
    long shift = 0;
    QString data;
    if (indexes) data = currTab->getTable()->model()->
            data(currTab->getTable()->model()->
                 index(indexes->at(0).row(), 0)).toString();
    else data = currentFile->getTitles()[0]->getSStart();
    long ind = currentFile->indexOf(data);
    TimeShiftDialog dialog(shift, *(currentFile->getTitles()[ind]), this);
    dialog.setModal(true);
    dialog.exec();
    if (shift == 0) return;

    try
    {
        if (indexes)
        {
            data = currTab->getTable()->model()->
                    data(currTab->getTable()->model()->
                         index(indexes->at(0).row(), 0)).toString();
            if (!processor->timeShift(data, shift)) throw 1;
            for (int i = 1; i < indexes->count(); i += 3)
            {
                data = currTab->getTable()->model()->
                        data(currTab->getTable()->model()->
                             index(indexes->at(i).row(), 0)).toString();
                processor->timeShift(data, shift);
            }
        }
        else
        {
            if (!processor->timeShift(0, shift)) throw 1;
            for (long i = 1; i < currentFile->getTitles().size(); i++)
                processor->timeShift(i, shift);
        }
    }
    catch (int e)
    {
        if (e == 1) status("Title can not statrt at negative time!");
        else status("Unknown error - title might be corrupted!");
        return;
    }

    status("*");
    processor->sort();
    currTab->refreshTitleList();
    updateWindowTitle();
    currTab->updateTitle();
    updateConsLable();
    status("Subtitle(s) shifted!");
}

void MainWindow::actionAutoConcat()
{
    if (currentFile->getTitles().size() == 0)
    {
        status("Nothing to concat in the empty file.");
        return;
    }

    long maxDist = DEFAULT_MAX_DISTANCE,
            maxLen = DEFAULT_MAX_LENGTH;
    AutoAdjustDialog dialog("Max title duration: ", "Autoc Concat", maxDist, maxLen, this);
    dialog.setModal(true);
    dialog.exec();
    if (maxDist < 1 || maxLen < 1) return;

    if (processor->autoConcat(maxDist, maxLen))
    {
        status("*");
        currTab->refreshTitleList();
        updateWindowTitle();
        currTab->updateTitle();
        updateConsLable();
        status("Auto concatenation finished!");
    }
    else status("Nothing to be done.");
}

void MainWindow::actionAutoSplit()
{
    if (currentFile->getTitles().size() == 0)
    {
        status("Nothing to split in the empty file.");
        return;
    }

    long maxDur = DEFAULT_MAX_DURATION,
            maxLen = DEFAULT_MAX_LENGTH;
    AutoAdjustDialog dialog("Max title duration: ", "Auto Devide", maxDur, maxLen, this);
    dialog.setModal(true);
    dialog.exec();
    if (maxDur < 1 || maxLen < 1) return;

    if (processor->autoSplit(maxDur, maxLen))
    {
        status("*");
        currTab->refreshTitleList();
        updateWindowTitle();
        currTab->updateTitle();
        updateConsLable();
        status("Auto split finished!");
    }
    else status("Nothing to be done.");
}

void MainWindow::actionConcatFiles()
{
    if (subtitleApp.getFilesCo() < 2)
    {
        status("Open more files to concat them.");
        return;
    }

    actionNew(); // Changes context to itself
    for (auto subs : subtitleApp.getSubtitles())
        if (subs != currentFile)
            processor->appendFile(*subs);

    status("*");
    currTab->refreshTitleList();
    updateWindowTitle();
    currTab->updateTitle();
    updateConsLable();
    status("Files concatenated!");
}

void MainWindow::actionSplitFile()
{
    if (currentFile->getTitles().size() == 0)
    {
        status("Can't split empty file.");
        return;
    }

    int co = 0;
    SplitFileDialog dialog(co, this);
    dialog.setModal(true);
    dialog.exec();

    if (co == -1) return;
    if (currentFile->getTitles().size() < co)
    {
        status("More files then subtitles.");
        return;
    }

    status("*");
    Subtitles *base = currentFile;
    int baseFileInd = currFileInd;
    long patchLen = currentFile->getTitles().size() / co;
    for (int i = 0; i < co - 1; i++)
    {
        actionNew(); // Changes context to itself
        processor->takeTitles(base, patchLen * i, patchLen);
        currTab->refreshTitleList();
        updateWindowTitle();
        currTab->updateTitle();
        updateConsLable();
    }
    actionNew(); // Changes context to itself
    processor->takeTitles(base, patchLen * (co - 1),
                          base->getTitles().size() - (co - 1) * patchLen);
    currTab->refreshTitleList();
    updateWindowTitle();
    currTab->updateTitle();
    updateConsLable();

    changeContext(baseFileInd);
    status("File splited!");
}

void MainWindow::actionAbout()
{
    QString message = "";
    if (currentFile->getFormat() == FORMATS::UNDEFINED)
        message = "No associated file with selected tab!";
    else
    {
        QString duration = currentFile->getTitles().back()->getSEnd();
        QString format = "SubRip";
        switch (currentFile->getFormat())
        {
        case FORMATS::MicroDVD: format = "MicroDVD"; break;
        case FORMATS::MPSub: format = "MPSub"; break;
        }
        QString subtitleCo = QString::number(currentFile->getTitles().size());
        QString size = QString::number(currentFile->getFileSize());
        message = "Information abot \nselected file:\n\n"
                "Duration:\t" + duration + "\n"
                "Format:\t" + format + "\n"
                "Subtitles:\t" + subtitleCo + "\n"
                "Size:\t\t" + size + " B";
    }
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
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

void MainWindow::on_FPSDoubleSpinBox_valueChanged(double)
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

void MainWindow::on_actionAdd_Subtitle_triggered()
{
    actionAddSubtitle();
}

void MainWindow::on_actionDelete_Subtitle_triggered()
{
    actionDelete();
}

void MainWindow::on_actionShift_Sellection_triggered()
{
    QModelIndexList indexes =
            currTab->getTable()->selectionModel()->
            selection().indexes();
    actionTimeShift(&indexes);
}

void MainWindow::on_actionShift_All_triggered()
{
    actionTimeShift(nullptr);
}

void MainWindow::on_actionAuto_Concat_triggered()
{
    actionAutoConcat();
}

void MainWindow::on_actionAuto_Devide_triggered()
{
    actionAutoSplit();
}

void MainWindow::on_actionConcat_Files_triggered()
{
    actionConcatFiles();
}

void MainWindow::on_actionAbout_File_triggered()
{
    actionAbout();
}

void MainWindow::on_actionSplit_File_triggered()
{
    actionSplitFile();
}

void MainWindow::updateConsLable()
{
    if (currentFile->getConsistency() == -1)
        ui->ConsLabel->setText("Subtitle is consistent.");
    else
        ui->ConsLabel->setText("Inconsistency at title " +
                               QString::number(currentFile->getConsistency() + 1) +
                               "!");
}
