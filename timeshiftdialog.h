#ifndef TIMESHIFTDIALOG_H
#define TIMESHIFTDIALOG_H

#include <QDialog>

namespace Ui {
class TimeShiftDialog;
}

class TimeShiftDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeShiftDialog(long&, QWidget *parent = 0);
    ~TimeShiftDialog();

private slots:
    void on_pushButton_2_clicked();

    void on_radioButton_3_toggled(bool checked);

    void on_radioButton_4_toggled(bool checked);

    void on_pushButton_clicked();

private:
    Ui::TimeShiftDialog *ui;

    long &shift;
};

#endif // TIMESHIFTDIALOG_H
