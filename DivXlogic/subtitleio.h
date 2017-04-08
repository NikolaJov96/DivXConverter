#ifndef SUBTITLEIO_H
#define SUBTITLEIO_H

#include "subtitles.h"

class SubtitleIO
{
    friend class Subtitles;
public:
    SubtitleIO();
    virtual void loadTitle(Subtitles&, const QString&) = 0;
    virtual void saveTitle(Subtitles&, const QString&) = 0;
};

#endif // SUBTITLEIO_H
