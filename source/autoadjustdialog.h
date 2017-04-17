#ifndef AUTOADJUSTDIALOG_H
#define AUTOADJUSTDIALOG_H

#include <QDialog>

namespace Ui {
class AutoAdjustDialog;
}

/*!
 * \brief The AutoAdjustDialog class
 *
 * Dialog class for obtaining parameters
 * for Auto Concat and Auto Devide actions
 */
class AutoAdjustDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoAdjustDialog(const QString&, const QString&,
                              long&, long&, QWidget *parent = 0);   /*!< Constructor with provided label text,
                                                                      window title and parameter references */
    ~AutoAdjustDialog();                                            /*!< Default destructor */

private slots:
    void on_pushButton_2_clicked();     /*!< Cancle button clicked */

    void on_pushButton_clicked();       /*!< Confirm button clicked */

private:
    Ui::AutoAdjustDialog *ui;           /*!< User interface interface */

    long &maxT, &maxL;                  /*!< Reference to parameter */
};

#endif // AUTOADJUSTDIALOG_H
