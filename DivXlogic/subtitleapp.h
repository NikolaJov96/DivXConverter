#ifndef SUBTITLEAPP_H
#define SUBTITLEAPP_H

#include "subtitleio.h"
#include "subtitles.h"

const int DEFAULT_FPS = 25;

class SubtitleApp
{
public:
    SubtitleApp();

    Subtitles &getSubtitles();

    bool isLoaded() const;

    void loadTitle(const QString&, FORMATS);
    void saveTitle(const QString&, FORMATS) const;
private:
    Subtitles subtitles;
    SRT SRTManager;
    MPSub MPSubManager;
    MicroDVD MicroDVDManager;

    void clearData();
    SubtitleIO const *getIOManager(FORMATS) const;
};

#endif // SUBTITLEAPP_H
