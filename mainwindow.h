#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

const QString PROGRAM_TITLE = "DivX Converter";

#include "DivXlogic/subtitleapp.h"
#include "DivXlogic/subtitleio.h"
#include "subtitleprocessing.h"

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class
 *
 * Main window class, containing all UI handlers
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);       /*!< Default constructor */
    ~MainWindow();                                  /*!< Default destructor */

private slots:

    void on_loadSubtitleButton_clicked();           /*!< Load subtitle button clicked handler */

    void on_saveSubtitleButton_clicked();           /*!< Save subtitle button clicked handler */

    void on_saveSubtitleAsButton_clicked();         /*!< Save subtitle as button clicked handler */

    void on_FPSDoubleSpinBox_valueChanged();        /*!< FPS spin box value changed handler */

    void on_FPSDoubleSpinBox_editingFinished();     /*!< FPS spin box editing finished handler */

    void on_editButton_clicked();                   /*!< Subtitle edit button clicked handler */

    void on_tableView_doubleClicked();              /*!< Tavle view double click handler */

    void on_searchLineEdit_editingFinished();

    void on_actionOpen_File_triggered();

    void on_actionSaveAsSRT_triggered();

    void on_actionSaveAsMPSub_triggered();

    void on_actionSaveAsMicroDVD_triggered();

    void on_actionExit_triggered();

    void on_actionClose_File_triggered();

    void on_actionEdit_Subtitle_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;                             /*!< User interface interface */
    SubtitleApp subtitleApp;                        /*!< SubtitleApp core application object */
    SubtitleProcessing processor;
    QString searchPhrase = "";                      /*!< Filter subtitles with this phrase */

    void actionLoad();                              /*!< Loading title form file acton */
    void actionSave();                              /*!< Saving title to file acton */
    void actionSaveAs(
            FORMATS format = FORMATS::UNDEFINED);   /*!< Saving title - as to file acton */
    void actionEdit();                              /*!< Edit selected title acton */
    void actionClose();                             /*!< Close opened file */
    int discardChangesDialog() const;               /*!< Show and return result of "Discard changes dialog" */
    void refreshTitleList();                        /*!< Refresh table view after changes to subtitle vector */
    void setUI(bool);                               /*!< Set enable state to parts of UI */
    void updateWindowTitle();                       /*!< Update window title to corespond to opened file */
};

#endif // MAINWINDOW_H
