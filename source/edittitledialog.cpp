#include "edittitledialog.h"
#include "ui_edittitledialog.h"

editTitleDialog::editTitleDialog(Subtitle *sub, QWidget *parent) :
    subtitle(sub),
    QDialog(parent),
    ui(new Ui::editTitleDialog)
{
    ui->setupUi(this);
    long start = subtitle->getStart();
    long end = subtitle->getEnd();

    ui->spinBox->setValue(start / 1000 / 60 / 60);
    ui->spinBox_2->setValue((start / 1000 / 60) % 60);
    ui->spinBox_3->setValue((start / 1000) % 60);
    ui->spinBox_4->setValue(start % 1000);

    ui->spinBox_5->setValue(end / 1000 / 60 / 60);
    ui->spinBox_6->setValue((end / 1000 / 60) % 60);
    ui->spinBox_7->setValue((end / 1000) % 60);
    ui->spinBox_8->setValue(end % 1000);

    ui->textEdit->setText(subtitle->getText());
}

editTitleDialog::~editTitleDialog()
{
    delete ui;
}

void editTitleDialog::on_pushButton_2_clicked()
{
    this->close();
}

void editTitleDialog::on_pushButton_clicked()
{
    subtitle->getStart() =
            ((ui->spinBox->value() * 60 +
              ui->spinBox_2->value()) * 60 +
            ui->spinBox_3->value()) * 1000 +
            ui->spinBox_4->value();

    subtitle->getEnd() =
            ((ui->spinBox_5->value() * 60 +
              ui->spinBox_6->value()) * 60 +
             ui->spinBox_7->value()) * 1000 +
            ui->spinBox_8->value();

    QString text = ui->textEdit->toPlainText();
    while (text.contains("\n\n")) text.replace("\n\n", "\n");
    if (text.startsWith("\n")) text = text.mid(1, text.length() - 1);
    if (text.endsWith("\n")) text = text.mid(0, text.length() - 1);
    subtitle->getText() = text;

    this->close();
}
