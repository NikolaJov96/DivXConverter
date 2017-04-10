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

    // save positions
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
    // restore position
}

void MainWindow::on_loadSubtitleButton_clicked()
{
    if (subtitleApp.isLoaded())
    {
        switch (discardChangesDialog()) {
        case QMessageBox::Save:
            break;
        case QMessageBox::Discard:
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
        subtitleApp.loadTitle(path, format);
        setUI(true);
    } catch (...) {}
    ui->loadSubtitleButton->setText("Load Subtitle");
    ui->FPSSpinBox->setValue(DEFAULT_FPS);
    ui->FPSlabel->setText("Current FPS:");
    refreshTitleList();
}

void MainWindow::on_saveSubtitleButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(
                this, "Save file", "",
                "Titles (*.srt *.sub *.txt);;All Files(*)");
    if (path.length() == 0) return;
    FORMATS format;
    if (ui->SRTRadioButton->isChecked()) format = FORMATS::SRT;
    else if (ui->MPSubRadioButton->isChecked()) format = FORMATS::MPSub;
    else if (ui->MicroDVDRadioButton->isChecked()) format = FORMATS::MicroDVD;
    else { /* throw wrong format */ }
    ui->saveSubtitleButton->setText("...");
    subtitleApp.saveTitle(path, format);
    ui->saveSubtitleButton->setText("Save Subtitle");
}

void MainWindow::setUI(bool state)
{
    ui->saveSubtitleButton->setEnabled(state);
    ui->saveTypeGroupBox->setEnabled(state);
    ui->FPSSpinBox->setEnabled(state);
}

void MainWindow::on_FPSSpinBox_valueChanged(int arg1)
{
    ui->FPSlabel->setText("Current FPS:*");
}

void MainWindow::on_FPSSpinBox_editingFinished()
{
    ui->FPSlabel->setText("Current FPS:");
    subtitleApp.getSubtitles().setFPS(
                ui->FPSSpinBox->value());
}
