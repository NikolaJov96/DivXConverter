#ifndef SUBTITLEAPP_H
#define SUBTITLEAPP_H

const double DEFAULT_FPS = 25.0;

#include "subtitleio.h"
#include "subtitles.h"

class SubtitleApp
{
public:
    SubtitleApp();

    Subtitles &getSubtitles();

    bool isLoaded() const;
    QString const &getFilePath() const;

    void loadTitle(const QString&, FORMATS, double = DEFAULT_FPS);
    void saveTitle(const QString&, FORMATS);
    void saveTitle();
private:
    Subtitles subtitles;
    SRT SRTManager;
    MPSub MPSubManager;
    MicroDVD MicroDVDManager;

    bool loaded = false;
    QString filePath = "";
    FORMATS fileType = FORMATS::UNDEFINED;

    void clearData();
    SubtitleIO const *getIOManager(FORMATS) const;
};

#endif // SUBTITLEAPP_H
