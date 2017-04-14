#include "timeshiftdialog.h"
#include "ui_timeshiftdialog.h"

TimeShiftDialog::TimeShiftDialog(long &s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeShiftDialog),
    shift(s)
{
    ui->setupUi(this);
}

TimeShiftDialog::~TimeShiftDialog()
{
    delete ui;
}

void TimeShiftDialog::on_pushButton_2_clicked()
{
    shift = 0;
    this->close();
}

void TimeShiftDialog::on_pushButton_clicked()
{
    if (ui->radioButton_3->isChecked())
        shift = ui->doubleSpinBox->value() * 1000;
    else if (ui->radioButton_4->isChecked())
        shift = ((ui->spinBox->value() * 60 +
                  ui->spinBox_2->value()) * 60 +
                 ui->spinBox_3->value()) * 1000 +
                ui->spinBox_4->value();

    if (shift == 0) this->close();

    if (ui->radioButton_2->isChecked()) shift = -shift;
    this->close();
}

void TimeShiftDialog::on_radioButton_3_toggled(bool checked)
{
    ui->doubleSpinBox->setEnabled(checked);
}

void TimeShiftDialog::on_radioButton_4_toggled(bool checked)
{
    ui->spinBox->setEnabled(checked);
    ui->spinBox_2->setEnabled(checked);
    ui->spinBox_3->setEnabled(checked);
    ui->spinBox_4->setEnabled(checked);
}
