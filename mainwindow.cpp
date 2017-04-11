#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DivXlogic/subtitleio.h"

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
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

void MainWindow::refreshTitleList()
{
    // reset table with subtitles
    long rows = subtitleApp.getSubtitles().subtitleCo();
    delete ui->tableView->model();
    QStandardItemModel *model =
            new QStandardItemModel(rows, 3);
    QStringList horizontalLabels =
        {"Start", "End", "Subtitle text"};
    model->setHorizontalHeaderLabels(horizontalLabels);

    // save vertical scrool positions
    long ind = 0;
    for (Subtitle* &row : subtitleApp.getSubtitles().getTitles())
    {
        // add title to table
        model->setItem(ind, 0, new QStandardItem(row->getSStart()));
        model->setItem(ind, 1, new QStandardItem(row->getSEnd()));
        model->setItem(ind, 2, new QStandardItem(row->getText()));
        ind++;
    }
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setColumnWidth(0, 75);
    ui->tableView->setColumnWidth(1, 75);
    ui->tableView->setColumnWidth(2, ui->tableView->width() - 200);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    // restore vertical scrool position
}

void MainWindow::on_loadSubtitleButton_clicked()
{
    if (subtitleApp.isLoaded())
    {
        switch (discardChangesDialog()) {
        case QMessageBox::Save:
            on_saveSubtitleButton_clicked();
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
        subtitleApp.loadTitle(path, format, ui->iFPSDoubleSpinBox->value());
        setUI(true);
    } catch (...) {}
    updateWindowTitle();
    ui->loadSubtitleButton->setText("Load Subtitle");
    ui->FPSDoubleSpinBox->setValue(subtitleApp.getSubtitles().getFPS());
    ui->FPSlabel->setText("Current FPS:");
    refreshTitleList();
}

void MainWindow::on_saveSubtitleButton_clicked()
{
    ui->saveSubtitleButton->setText("...");
    try
    {
        subtitleApp.saveTitle();
    }
    catch (...) { /* show message - umable to save */ }
    ui->saveSubtitleButton->setText("Save Subtitle");
}

void MainWindow::on_saveSubtitleAsButton_clicked()
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

void MainWindow::setUI(bool state)
{
    ui->saveSubtitleButton->setEnabled(state);
    ui->saveSubtitleAsButton->setEnabled(state);
    ui->saveTypeGroupBox->setEnabled(state);
    ui->FPSDoubleSpinBox->setEnabled(state);
}

void MainWindow::updateWindowTitle()
{
    if (subtitleApp.isLoaded())
        this->setWindowTitle(PROGRAM_TITLE + " | " +
                             subtitleApp.getFilePath());
    else
        this->setWindowTitle(PROGRAM_TITLE);
}

void MainWindow::on_FPSDoubleSpinBox_valueChanged(double arg1)
{
    ui->FPSlabel->setText("Current FPS:*");
}

void MainWindow::on_FPSDoubleSpinBox_editingFinished()
{
    ui->FPSlabel->setText("Current FPS:");
    subtitleApp.getSubtitles().setFPS(
                ui->FPSDoubleSpinBox->value());
}
