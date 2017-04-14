#ifndef SUBTITLEPROCESSING_H
#define SUBTITLEPROCESSING_H

#include <QWidget>

const int DEF_CONCAT_DIST = 1000;
const int DEF_CONCAT_LEN = 1000;
const int DEF_SEL_LEN = 1;

#include "DivXlogic/subtitles.h"

class SubtitleProcessing
{
public:
    SubtitleProcessing(Subtitles*, QWidget*);

    bool editTitle(long) const;
    bool editTitle(const QString&) const;
    bool addSubtitle() const;
    bool deleteSubtitle(long) const;
    bool deleteSubtitle(const QString&) const;
    bool timeShift(long, long) const;
    bool timeShift(const QString&, long) const;
    void autoConcat();
private:
    QWidget *window = nullptr;
    Subtitles *subs = nullptr;
};

#endif // SUBTITLEPROCESSING_H
