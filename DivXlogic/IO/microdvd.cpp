#include "../subtitleio.h"

MicroDVD::MicroDVD() {}

void MicroDVD::loadTitle(Subtitles& subs, const QString&p) const
{
    // load text file in MicroDVD format form path p
    // parse it and store to subs
}

void MicroDVD::saveTitle(const Subtitles& subs, const QString&p) const
{
    // convert titles from subs to MicroDVD format
    // save it to file with path p
}
