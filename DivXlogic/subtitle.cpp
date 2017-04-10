#include "subtitle.h"

Subtitle::Subtitle(const QString &t, long s, long e) :
    text(t), start(s), end(e) {}

QString &Subtitle::getText() { return text; }

long &Subtitle::getStart() { return start; }

QString Subtitle::getSStart() const
{
    return longToTime(start);
}

long &Subtitle::getEnd() { return end; }

QString Subtitle::getSEnd() const
{
    return longToTime(end);
}

QString Subtitle::longToTime(long ms) const
{
    return QString("%1").arg(ms / 1000 / 60 / 60, 2, 10, QChar('0')) + ":" +
            QString("%1").arg((ms / 1000 / 60) % 60, 2, 10, QChar('0')) + ":" +
            QString("%1").arg((ms / 1000) % 60, 2, 10, QChar('0')) + "," +
            QString("%1").arg(ms % 1000, 3, 10, QChar('0'));
}
