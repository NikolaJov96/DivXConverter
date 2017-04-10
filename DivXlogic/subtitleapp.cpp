#include "subtitleapp.h"

SubtitleApp::SubtitleApp() {}

Subtitles &SubtitleApp::getSubtitles() { return subtitles; }

bool SubtitleApp::isLoaded() const
{
    return !subtitles.isEmpty();
}

void SubtitleApp::loadTitle(const QString &path, FORMATS format)
{
    if (format == FORMATS::UNDEFINED) { /* throw undefined format */ }
    clearData();
    getIOManager(format)->loadTitle(subtitles, path);
}

void SubtitleApp::saveTitle(const QString &path, FORMATS format) const
{
    if (format == FORMATS::UNDEFINED) { /* throw undefined format */ }
    getIOManager(format)->saveTitle(subtitles, path);
}

void SubtitleApp::clearData()
{
    for (auto row : subtitles.getTitles()) delete row;
    subtitles.getTitles().clear();
    subtitles.setFPS(DEFAULT_FPS);
}

SubtitleIO const *SubtitleApp::getIOManager(FORMATS format) const
{
    switch (format) {
    case FORMATS::SRT: return &SRTManager; break;
    case FORMATS::MPSub: return &MPSubManager; break;
    case FORMATS::MicroDVD: return &MicroDVDManager; break;
    case FORMATS::UNDEFINED: return nullptr;
    }
    return nullptr;
}

