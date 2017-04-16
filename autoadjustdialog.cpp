#include "autoadjustdialog.h"
#include "ui_autoadjustdialog.h"

AutoAdjustDialog::AutoAdjustDialog(const QString &label, const QString &title,
                                   long &mT, long &mL, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoAdjustDialog),
    maxT(mT),
    maxL(mL)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->label->setText(label);
    ui->doubleSpinBox->setValue(mT / 1000.0);
    ui->spinBox->setValue(mL);
}

AutoAdjustDialog::~AutoAdjustDialog()
{
    delete ui;
}

void AutoAdjustDialog::on_pushButton_2_clicked()
{
    maxT = maxL = -1;
    this->close();
}

void AutoAdjustDialog::on_pushButton_clicked()
{
    maxT = ui->doubleSpinBox->value() * 1000;
    maxL = ui->spinBox->value();
    this->close();
}
