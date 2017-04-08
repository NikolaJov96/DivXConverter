#ifndef SUBTITLEIO_H
#define SUBTITLEIO_H

#include "subtitles.h"

class SubtitleIO
{
    friend class Subtitles;
public:
    virtual void loadTitle(Subtitles&, const QString&) const = 0;
    virtual void saveTitle(Subtitles&, const QString&) const = 0;
};

class SRT : public SubtitleIO
{
public:
    void loadTitle(Subtitles&, const QString&) const;
    void saveTitle(Subtitles&, const QString&) const;
};

class MicroDVD : public SubtitleIO
{
public:
    void loadTitle(Subtitles&, const QString&) const;
    void saveTitle(Subtitles&, const QString&) const;
};

class MPSub : public SubtitleIO
{
public:
    void loadTitle(Subtitles&, const QString&) const;
    void saveTitle(Subtitles&, const QString&) const;
};

#endif // SUBTITLEIO_H
