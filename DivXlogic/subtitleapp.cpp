#include "subtitleapp.h"

SubtitleApp::~SubtitleApp()
{
    for (auto file : files) delete file;
    files.clear();
}

Subtitles *SubtitleApp::getSubtitles(int ind)
{
    return files[ind];
}

std::vector<Subtitles*> &SubtitleApp::getSubtitles()
{
    return files;
}

int SubtitleApp::getFilesCo() { return files.size(); }

void SubtitleApp::newTitle()
{
    files.push_back(new Subtitles());
}

void SubtitleApp::loadTitle(const QString &path,
                            FORMATS format, double fps)
{
    if (format == FORMATS::UNDEFINED) { /* throw unable to load */ }
    Subtitles *subs = new Subtitles();
    try
    {
        getIOManager(format)->loadTitle(*subs, path, fps);
    }
    catch (...)
    {
        delete subs;
        /* throw unable to load */
    }
    subs->setFilePath(path);
    subs->setFormat(format);
    subs->setFPS(fps);
    subs->setEdited(false);
    files.push_back(subs);
}

void SubtitleApp::saveTitle(const QString &path, FORMATS format, int ind)
{
    if (format == FORMATS::UNDEFINED ||
            ind < 0 || ind >= files.size()) { /* throw unable to save */ }
    try
    {
        getIOManager(format)->saveTitle(*files[ind], path);
    }
    catch (...) { /* throw unable to save */ return; }
    files[ind]->setFilePath(path);
    files[ind]->setFormat(format);
    files[ind]->setEdited(false);
}

void SubtitleApp::saveTitle(int ind)
{
    if (ind < 0 || ind >= files.size()) { /* throw unable to save */ };
    saveTitle(files[ind]->getFilePath(),
              files[ind]->getFormat(), ind);
}

void SubtitleApp::closeFile(int ind)
{
    if (ind < 0 || ind >= files.size()) return;
    delete files[ind];
    files.erase(files.begin() + ind);
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

