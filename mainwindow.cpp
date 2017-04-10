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
    FORMATS format = SubtitleIO::detect(path);
    subtitleApp.loadTitle(path, format);
    refreshTitleList();
}
