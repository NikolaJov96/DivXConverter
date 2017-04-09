#ifndef SUBTITLE_H
#define SUBTITLE_H

#include <QString>

class Subtitle
{
public:
    Subtitle(const QString&, long, long);

    QString &getText();
    long &getStart();
    long &getEnd();

private:
    QString text;
    // absolute start and end time in ms
    long start, end;
};

#endif // SUBTITLE_H
