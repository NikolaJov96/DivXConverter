#include "subtitleapp.h"

SubtitleApp::~SubtitleApp() { clearData(); }

Subtitles &SubtitleApp::getSubtitles()
{
    // throw if not loaded
    // if (!isLoaded()) throw ...;
    return subtitles;
}

bool SubtitleApp::isLoaded() const { return loaded; }

QString const &SubtitleApp::getFilePath() const
{
    // throw if not loaded
    // if (!isLoaded()) throw ...;
    return filePath;
}

void SubtitleApp::loadTitle(const QString &path, FORMATS format, double fps)
{
    if (format == FORMATS::UNDEFINED) { /* throw unable to load */ }
    clearData();
    try
    {
        getIOManager(format)->loadTitle(subtitles, path, fps);
    }
    catch (...) { /* throw unable to load */ }
    loaded = true;
    filePath = path;
    fileType = format;
}

void SubtitleApp::saveTitle(const QString &path, FORMATS format)
{
    if (format == FORMATS::UNDEFINED) { /* throw unable to save */ }
    try
    {
        getIOManager(format)->saveTitle(subtitles, path);
        filePath = path;
        fileType = format;
    }
    catch (...) { /* throw unable to save */ }
}

void SubtitleApp::saveTitle()
{
    saveTitle(filePath, fileType);
}

void SubtitleApp::clearData()
{
    for (auto row : subtitles.getTitles()) delete row;
    subtitles.getTitles().clear();
    loaded = false;
    filePath = "";
    fileType = FORMATS::UNDEFINED;
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

