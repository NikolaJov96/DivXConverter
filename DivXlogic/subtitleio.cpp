#include "subtitleio.h"

FORMATS SubtitleIO::detect(const QString&)
{
    // detect subtitle file format
    // do not check if file is actualy valid
    // if no supported types detected - throw
    return FORMATS::SRT;
}
