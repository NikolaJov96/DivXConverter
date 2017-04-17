#include "subtitle.h"

Subtitle::Subtitle(const QString &t, long s, long e) :
    text(t), start(s), end(e) {}

QString &Subtitle::getText() { return text; }

QString const &Subtitle::getText() const { return text; }

long &Subtitle::getStart() { return start; }

long &Subtitle::getEnd() { return end; }

long Subtitle::getStart() const { return start; }

long Subtitle::getEnd() const { return end; }

QString Subtitle::getSStart() const
{
    return longToTime(start);
}

QString Subtitle::getSEnd() const
{
    return longToTime(end);
}

bool Subtitle::operator==(const Subtitle &sub) const
{
    if (sub.text != text) return false;
    if (sub.start != start) return false;
    if (sub.end != end) return false;
    return true;
}

QString Subtitle::longToTime(long ms) const
{
    return QString("%1").arg(ms / 1000 / 60 / 60, 2, 10, QChar('0')) + ":" +
            QString("%1").arg((ms / 1000 / 60) % 60, 2, 10, QChar('0')) + ":" +
            QString("%1").arg((ms / 1000) % 60, 2, 10, QChar('0')) + "," +
            QString("%1").arg(ms % 1000, 3, 10, QChar('0'));
}
