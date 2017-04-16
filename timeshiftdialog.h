#ifndef TIMESHIFTDIALOG_H
#define TIMESHIFTDIALOG_H

#include <QDialog>

#include <DivXlogic/subtitle.h>

namespace Ui {
class TimeShiftDialog;
}

class TimeShiftDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeShiftDialog(long&, const Subtitle&, QWidget *parent = 0);
    ~TimeShiftDialog();

private slots:
    void on_pushButton_2_clicked();                 /*!< Canel button clicked */

    void on_pushButton_clicked();                   /*!< Confirm button clicked */

    void on_radioButton_3_toggled(bool checked);    /*!< Delta time in secondes radio button */

    void on_radioButton_4_toggled(bool checked);    /*!< Full time format radio button */

    void on_radioButton_5_toggled(bool checked);    /*!< Delta option radio button */

    void on_radioButton_6_toggled(bool checked);    /*!< Absolute time option */

private:
    Ui::TimeShiftDialog *ui;

    const Subtitle &initial;
    long &shift;
};

#endif // TIMESHIFTDIALOG_H
