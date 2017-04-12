#ifndef SUBTITLEPROCESSING_H
#define SUBTITLEPROCESSING_H

#include "DivXlogic/subtitles.h"

class SubtitleProcessing
{
public:
    void setSubtitles(Subtitles*);
private:
    Subtitles *subs = nullptr;
};

#endif // SUBTITLEPROCESSING_H
