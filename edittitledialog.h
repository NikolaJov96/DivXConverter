#ifndef EDITTITLEDIALOG_H
#define EDITTITLEDIALOG_H

#include <QDialog>

#include "DivXlogic/subtitle.h"

namespace Ui {
class editTitleDialog;
}

/*!
 * \brief The editTitleDialog class
 *
 * Subtitle edit dialod class
 */
class editTitleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editTitleDialog(Subtitle*, QWidget *parent = 0);   /*!< Constructor with provided Subtitle to edit */
    ~editTitleDialog();                                         /*!< Default destructor */

private slots:
    void on_pushButton_2_clicked();                             /*!< Save button - save shanges and close dialog */

    void on_pushButton_clicked();                               /*!< Cancle button - discard changes and close dialog */

private:
    Ui::editTitleDialog *ui;                                    /*!< User interface interface */
    Subtitle *subtitle;                                         /*!< Pointer to the Subtitle that is being edited */
};

#endif // EDITTITLEDIALOG_H
