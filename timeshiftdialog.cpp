#include "timeshiftdialog.h"
#include "ui_timeshiftdialog.h"

TimeShiftDialog::TimeShiftDialog(long &s, const Subtitle &init, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeShiftDialog),
    initial(init),
    shift(s)
{
    ui->setupUi(this);
    ui->spinBox_5->setValue(initial.getStart() / 1000 / 60 / 60);
    ui->spinBox_6->setValue((initial.getStart() / 1000 / 60) % 60);
    ui->spinBox_7->setValue((initial.getStart() / 1000) % 60);
    ui->spinBox_8->setValue(initial.getStart() % 1000);
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
    if (ui->radioButton_5->isChecked())
    {
        if (ui->radioButton_3->isChecked())
            shift = ui->doubleSpinBox->value() * 1000;
        else if (ui->radioButton_4->isChecked())
            shift = ((ui->spinBox->value() * 60 +
                      ui->spinBox_2->value()) * 60 +
                     ui->spinBox_3->value()) * 1000 +
                    ui->spinBox_4->value();
        if (ui->radioButton_2->isChecked()) shift = -shift;
    }
    else if (ui->radioButton_6->isChecked())
    {
        shift = ((ui->spinBox_5->value() * 60 +
                  ui->spinBox_6->value()) * 60 +
                 ui->spinBox_7->value()) * 1000 +
                ui->spinBox_8->value();
        shift -= initial.getStart();
    }

    if (shift == 0) this->close();
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

void TimeShiftDialog::on_radioButton_5_toggled(bool checked)
{
    ui->groupBox->setEnabled(checked);
    ui->groupBox_2->setEnabled(checked);
}

void TimeShiftDialog::on_radioButton_6_toggled(bool checked)
{
    ui->spinBox_5->setEnabled(checked);
    ui->spinBox_6->setEnabled(checked);
    ui->spinBox_7->setEnabled(checked);
    ui->spinBox_8->setEnabled(checked);
}
