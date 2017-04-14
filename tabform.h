#ifndef TABFORM_H
#define TABFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>

#include "DivXlogic/subtitles.h"
#include "subtitleprocessing.h"

namespace Ui {
class TabForm;
}

class TabForm : public QWidget
{
    Q_OBJECT

public:
    explicit TabForm(Subtitles*, QWidget *parent = 0);

    QString const &getSearchPhrase() const;
    Subtitles *getFile();
    QTableView *getTable();
    SubtitleProcessing *getProcessor();

    void setSearhPhrase(const QString&);
    void setFile(Subtitles*);

    void updateTitle();
    void refreshTitleList();
    ~TabForm();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::TabForm *ui;
    QString searchPhrase = "";           /*!< Filter subtitles with this phrase */
    Subtitles *file;                     /*!< Currently selected file form subtitleApp */
    SubtitleProcessing processor;
    QStandardItemModel *model = nullptr; /*!< Current table view item model */
};

#endif // TABFORM_H
