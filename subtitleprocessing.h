#ifndef SUBTITLEPROCESSING_H
#define SUBTITLEPROCESSING_H

const int DEF_CONCAT_DIST = 1000;
const int DEF_CONCAT_LEN = 1000;
const int DEF_SEL_LEN = 1;

#include "DivXlogic/subtitles.h"

class SubtitleProcessing
{
public:
    void setSubtitles(Subtitles*);
    void timeShift(long start, long end = -1) const;
    void autoConcat();
private:
    Subtitles *subs = nullptr;

};

#endif // SUBTITLEPROCESSING_H
