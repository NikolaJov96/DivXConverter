#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QCloseEvent>

const QString PROGRAM_TITLE = "DivX Converter";     /*!< Program title */

#include "DivXlogic/subtitleapp.h"
#include "DivXlogic/subtitleio.h"
#include "DivXlogic/subtitleprocessing.h"
#include "tabform.h"

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
    explicit MainWindow(int, char**, QWidget *parent = 0);       /*!< Default constructor */
    ~MainWindow();                                  /*!< Default destructor */

    void actionEdit();                              /*!< Edit selected title acton */

private slots:

    void on_loadSubtitleButton_clicked();           /*!< Load subtitle button clicked handler */

    void on_saveSubtitleButton_clicked();           /*!< Save subtitle button clicked handler */

    void on_saveSubtitleAsButton_clicked();         /*!< Save subtitle as button clicked handler */

    void on_FPSDoubleSpinBox_valueChanged(double);  /*!< FPS spin box value changed handler */

    void on_FPSDoubleSpinBox_editingFinished();     /*!< FPS spin box editing finished handler */

    void on_editButton_clicked();                   /*!< Subtitle edit button clicked handler */

    void on_searchLineEdit_editingFinished();       /*!< Refresh subtitle table on serch phrase edited */

    void on_actionOpen_File_triggered();            /*!< Load File menu item clicked handler */

    void on_actionSaveAsSRT_triggered();            /*!< Save As SRT menu item clicked handler */

    void on_actionSaveAsMPSub_triggered();          /*!< Save As MPSub menu item clicked handler */

    void on_actionSaveAsMicroDVD_triggered();       /*!< Save As MicroDVD menu item clicked handler */

    void on_actionExit_triggered();                 /*!< Exit menu item clicked handler */

    void on_actionClose_File_triggered();           /*!< Close File menu item clicked handler */

    void on_actionEdit_Subtitle_triggered();        /*!< Edit subtitle menu item clicked handler */

    void on_actionSave_triggered();                 /*!< Save menu item clicked handler */

    void on_tabWidget_currentChanged(int index);    /*!< Tab changed */

    void on_actionNew_File_triggered();             /*!< New File menu item clicked handler */

    void on_actionAdd_Subtitle_triggered();         /*!< Add Subtitle menu item clicked handler */

    void on_actionDelete_Subtitle_triggered();      /*!< Delete Subtitle menu item clicked handler */

    void on_actionShift_Sellection_triggered();     /*!< Shift Sellection menu itam clicked handler */

    void on_actionShift_All_triggered();            /*!< Shft All menu itam clicked handler */

    void on_actionAuto_Concat_triggered();          /*!< Auto Concat menu item clicked handler */

    void on_actionAuto_Devide_triggered();          /*!< Auto Devide menu item clicked handler */

    void on_actionConcat_Files_triggered();         /*!< Concat Files menu item clicked handler */

    void on_actionAbout_File_triggered();

private:
    Ui::MainWindow *ui;                             /*!< User interface interface */
    SubtitleApp subtitleApp;                        /*!< SubtitleApp core application object */

    SubtitleProcessing *processor = nullptr;        /*!< Object responsible for title manipulation */
    Subtitles *currentFile = nullptr;               /*!< Poiter to the Subtitles of the sellected tab */
    int currFileInd = 0;                            /*!< Index of the sellected tab */
    TabForm *currTab = nullptr;                     /*!< Pointer to the current tab in tabWidget */

    void closeEvent(QCloseEvent*);                  /*!< Overrides QMainWindow method for closing the window */
    void changeContext(int);                        /*!< Switches to the desited tab (file) */
    void actionNew();                               /*!< Creates new Subtitles and tab */
    void actionLoad();                              /*!< Loads title form file to the new tab */
    void actionSave();                              /*!< Saves title to file */
    void actionSaveAs(
            FORMATS format = FORMATS::UNDEFINED);   /*!< Performs Save As to file */
    void actionAddSubtitle();                       /*!< Adds Subtitle to current Subtitles */
    void actionClose();                             /*!< Closes opened file */
    void actionDelete();                            /*!< Removes Subtitle from Subtitles */
    void actionTimeShift(QModelIndexList*);         /*!< Shifts start and end of desired Subtitle */
    void actionAutoConcat();                        /*!< Concats suitable titles */
    void actionAutoSplit();                         /*!< Splits suitable titles */
    void actionConcatFiles();                       /*!< Concats all opened files to the new one */
    void actionAbout();                             /*!< Displays information about opened file */
    int discardChangesDialog() const;               /*!< Show and return result of "Discard changes dialog" */
    void refreshTitleList();                        /*!< Refresh table view after changes to subtitle vector */
    void updateWindowTitle();                       /*!< Update window title to corespond to opened file */
    void status(const QString&);                    /*!< Show message in status bar */
};

#endif // MAINWINDOW_H
