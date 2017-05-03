#ifndef SPLITFILEDIALOG_H
#define SPLITFILEDIALOG_H

#include <QDialog>

#include "DivXlogic/subtitles.h"

namespace Ui {
class SplitFileDialog;
}

class SplitFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SplitFileDialog(int&, QWidget *parent = 0);
    ~SplitFileDialog();

private slots:
    void on_pushButton_clicked();                   /*!< Confirm button clicked */

    void on_pushButton_2_clicked();                 /*!< Cancel button clicked */

private:
    Ui::SplitFileDialog *ui;                        /*!< User interface interface */
    int &co;                                        /*!< Number of new files */
};

#endif // SPLITFILEDIALOG_H
