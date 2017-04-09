#ifndef SUBTITLEAPP_H
#define SUBTITLEAPP_H

#include "subtitleio.h"
#include "subtitles.h"

class SubtitleApp
{
public:
    SubtitleApp();

    bool isLoaded() const;

    void loadTitle(const QString&, FORMATS);
private:
    Subtitles subtitles;
    SubtitleIO *IOmanager = nullptr;
    SRT SRTManager;
    MPSub MPSubManager;
    MicroDVD MicroDVDManager;
};

#endif // SUBTITLEAPP_H
