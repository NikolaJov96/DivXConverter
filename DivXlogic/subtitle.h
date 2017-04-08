#ifndef SUBTITLE_H
#define SUBTITLE_H

#include <QString>

class Subtitle
{
public:
    Subtitle(const QString&, long, long);

    long &getStart();
    long &getEnd();

private:
    QString text;
    long start, end;
};

#endif // SUBTITLE_H
