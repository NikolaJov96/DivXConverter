#ifndef SUBTITLEIO_H
#define SUBTITLEIO_H

#include <QString>

#include "subtitles.h"

enum class FORMATS {UNDEFINED, SRT, MPSub, MicroDVD};

class SubtitleIO
{
public:
    virtual void loadTitle(Subtitles&, const QString&) const = 0;
    virtual void saveTitle(const Subtitles&, const QString&) const = 0;
    static FORMATS detect(const QString&);
};

class SRT : public SubtitleIO
{
public:
    SRT();
    void loadTitle(Subtitles&, const QString&) const;
    void saveTitle(const Subtitles&, const QString&) const;
private:
    long timeToLong(const QString&) const;
    QString longToTime(long) const;
};

class MicroDVD : public SubtitleIO
{
public:
    MicroDVD();
    void loadTitle(Subtitles&, const QString&) const;
    void saveTitle(const Subtitles&, const QString&) const;
private:
    long timeToLong(const QString&) const;
    QString longToTime(long) const;
};

class MPSub : public SubtitleIO
{
    friend class Subtitles;
public:
    MPSub();
    void loadTitle(Subtitles&, const QString&) const;
    void saveTitle(const Subtitles&, const QString&) const;
private:
    long timeToLong(const QString&) const;
    QString longToTime(long) const;
};

#endif // SUBTITLEIO_H
