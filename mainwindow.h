#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

const QString PROGRAM_TITLE = "DivX Converter";

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

    void on_saveSubtitleAsButton_clicked();

    void on_FPSDoubleSpinBox_valueChanged(double arg1);

    void on_FPSDoubleSpinBox_editingFinished();

private:
    Ui::MainWindow *ui;
    SubtitleApp subtitleApp;

    int discardChangesDialog() const;
    void refreshTitleList();
    void setUI(bool);
    void updateWindowTitle();
};

#endif // MAINWINDOW_H
