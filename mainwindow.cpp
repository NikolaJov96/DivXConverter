#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DivXlogic/subtitleio.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

void MainWindow::on_loadSubtitleButton_clicked()
{
    if (SubtitleApp.isLoaded())
    {
        switch (discardChangesDialog()) {
        case QMessageBox::Save:
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            break;
        }
    }
    QString path = QFileDialog::getOpenFileName(
                this, "Open file", "",
                "Titles (*.srt *.sub *.txt);;All Files(*)");
    ui->label->setText(path);
    FORMATS format = SubtitleIO::detect(path);
    SubtitleApp.loadTitle(path, format);
}
