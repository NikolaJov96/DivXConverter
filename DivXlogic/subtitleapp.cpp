#include "subtitleapp.h"

SubtitleApp::SubtitleApp() {}

bool SubtitleApp::isLoaded() const
{
    return !(subtitles.isEmpty() || !IOmanager);
}

void SubtitleApp::loadTitle(const QString &path, FORMATS format)
{
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
