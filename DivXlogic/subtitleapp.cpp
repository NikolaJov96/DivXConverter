#include "subtitleapp.h"

SubtitleApp::SubtitleApp() {}

Subtitles &SubtitleApp::getSubtitles() { return subtitles; }

bool SubtitleApp::isLoaded() const
{
    return !(subtitles.isEmpty() || !IOmanager);
}

void SubtitleApp::loadTitle(const QString &path, FORMATS format)
{
    for (auto row : subtitles.getTitles()) delete row;
    subtitles.getTitles().clear();
    switch (format) {
    case FORMATS::SRT:
        IOmanager = &SRTManager; break;
    case FORMATS::MPSub:
        IOmanager = &MPSubManager; break;
    case FORMATS::MicroDVD:
        IOmanager = &MicroDVDManager; break;
    }
    IOmanager->loadTitle(subtitles, path);
}
