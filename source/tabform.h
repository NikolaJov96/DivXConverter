#ifndef TABFORM_H
#define TABFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>


#include "mainwindow.h"
#include "DivXlogic/subtitles.h"
#include "DivXlogic/subtitleprocessing.h"

namespace Ui {
class TabForm;
}

class TabForm : public QWidget
{
    Q_OBJECT

public:
    explicit TabForm(Subtitles*, MainWindow *parent);   /*!< Constructor with provied Subtitles* */
    ~TabForm();                                         /*!< Defaut destructor */

    QString const &getSearchPhrase() const;             /*!< Returns saved search phrase */
    Subtitles *getFile();                               /*!< Retunrs associated Subtitles* */
    QTableView *getTable();                             /*!< Returns pointer to the tableWied */
    SubtitleProcessing *getProcessor();                 /*!< Returns associated subtitle processor */

    void setSearhPhrase(const QString&);                /*!< Stets new search phrase */
    void setFile(Subtitles*);                           /*!< Sets new associated file */

    void updateTitle();                                 /*!< Sets tab title */
    void refreshTitleList();                            /*!< Refreshes tableView when content changes */

private slots:
    void on_tableView_doubleClicked(
            const QModelIndex &index);                  /*!< Inits edit action for selected title */

private:
    Ui::TabForm *ui;                        /*!< User interface interface */
    MainWindow *mainWindow;
    QString searchPhrase = "";              /*!< Filter subtitles with this phrase */
    Subtitles *file;                        /*!< Currently selected file form subtitleApp */
    SubtitleProcessing processor;           /*!< Processor for managing associated Subtitles */
    QStandardItemModel *model = nullptr;    /*!< Current tableView item model */
};

#endif // TABFORM_H
