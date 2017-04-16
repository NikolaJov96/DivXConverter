#ifndef SUBTITLEPROCESSING_H
#define SUBTITLEPROCESSING_H

#include <QWidget>

const int DEF_CONCAT_DIST = 1000;
const int DEF_CONCAT_LEN = 1000;
const int DEF_SEL_LEN = 1;

#include "DivXlogic/subtitles.h"

/*!
 * \brief The SubtitleProcessing class
 *
 * Class that contains methods for
 * managing subtitles
 */
class SubtitleProcessing
{
public:
    SubtitleProcessing(Subtitles*, QWidget*);       /*!< Constructor with provided file and parent QWidget */

    void sort() const;                              /*!< Sorts the Subtitles* vector */
    bool editTitle(long) const;                     /*!< Edits the title with provided index */
    bool editTitle(const QString&) const;           /*!< Edits the title with provided start time */
    bool addSubtitle() const;                       /*!< Adds new Subtitle */
    bool deleteSubtitle(long) const;                /*!< Deletes subtitle with provided index */
    bool deleteSubtitle(const QString&) const;      /*!< Deletes subritle with provided start time */
    bool timeShift(long, long) const;               /*!< Shifts time of appearance for the title */
    bool timeShift(const QString&, long) const;     /*!< Shifts time of appearance for the title */
    bool autoConcat(long, long);                    /*!< Automatically merges suitable subtitles */
    bool autoSplit(long, long);                     /*!< Automatically splits suitable subtitles */
    bool appendFile(const Subtitles&);              /*!< Appends provided file to the current one */
private:
    QWidget *window = nullptr;                      /*!< Pointer to the parent window */
    Subtitles *subs = nullptr;                      /*!< Pointer to the target Subtitles file */
};

#endif // SUBTITLEPROCESSING_H
