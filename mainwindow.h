#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "DivXlogic/subtitleapp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_loadSubtitleButton_clicked();

private:
    Ui::MainWindow *ui;
    SubtitleApp SubtitleApp;

    int discardChangesDialog() const;
};

#endif // MAINWINDOW_H
