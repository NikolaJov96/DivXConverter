#include "splitfiledialog.h"
#include "ui_splitfiledialog.h"

SplitFileDialog::SplitFileDialog(int &c, QWidget *parent) :
    QDialog(parent),
    co(c),
    ui(new Ui::SplitFileDialog)
{
    ui->setupUi(this);
}

SplitFileDialog::~SplitFileDialog()
{
    delete ui;
}

void SplitFileDialog::on_pushButton_clicked()
{
    co = ui->spinBox->value();
    this->close();
}

void SplitFileDialog::on_pushButton_2_clicked()
{
    co = -1;
    this->close();
}
