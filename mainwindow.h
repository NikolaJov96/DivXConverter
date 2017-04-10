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

    void on_saveSubtitleButton_clicked();

    void on_FPSSpinBox_valueChanged(int arg1);

    void on_FPSSpinBox_editingFinished();

private:
    Ui::MainWindow *ui;
    SubtitleApp subtitleApp;

    int discardChangesDialog() const;
    void refreshTitleList();
    void setUI(bool);
};

#endif // MAINWINDOW_H
