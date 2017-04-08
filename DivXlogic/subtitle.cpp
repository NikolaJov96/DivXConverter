#include "subtitle.h"

Subtitle::Subtitle(const QString &t, long s, long e) :
    text(t), start(s), end(e) {}

long &Subtitle::getStart() { return start; }

long &Subtitle::getEnd() { return end; }
