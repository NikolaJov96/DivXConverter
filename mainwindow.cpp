#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_loadSubtitleButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this, "Open file", "",
                "Titles (*.srt *.sub *.txt);;All Files(*)");
    ui->label->setText(path);
}
