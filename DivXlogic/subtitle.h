#ifndef SUBTITLE_H
#define SUBTITLE_H

#include <QString>

class Subtitle
{
public:
    Subtitle(const QString&, long, long);

    QString &getText();
    long &getStart();
    QString getSStart() const;
    long &getEnd();
    QString getSEnd() const;

private:
    QString text;
    // absolute start and end time in ms
    long start, end;

    QString longToTime(long) const;
};

#endif // SUBTITLE_H
