#include "subtitles.h"

Subtitles::~Subtitles()
{
    for (auto row : subtitles) delete row;
    subtitles.clear();
}

std::vector<Subtitle*> &Subtitles::getTitles() { return subtitles; }

std::vector<Subtitle*> const &Subtitles::getTitles() const
{
    return subtitles;
}

double Subtitles::getFPS() const { return FPS; }

QString const &Subtitles::getFilePath() const
{
    return filePath;
}

FORMATS Subtitles::getFormat() const { return fileType; }

long Subtitles::getConsistency() const { return consistent; }

long long Subtitles::getFileSize() const { return fileSize; }

bool Subtitles::isEdited() const { return edited; }

void Subtitles::setFPS(double fps) {
    if (fps == FPS) return;
    FPS = fps;
    edited = true;
}

void Subtitles::setFilePath(const QString &path) {
    if (filePath == path) return;
    filePath = path;
    edited = true;
}

void Subtitles::setFormat(FORMATS format) { fileType = format; }

void Subtitles::setEdited(bool e) { edited = e; }

void Subtitles::setConsistency(long c) { consistent = c; }

void Subtitles::setFileSize(long long s) { fileSize = s; }

long Subtitles::indexOf(const QString &time) const
{
    long row = 0;
    for (auto it = subtitles.begin();
         it != subtitles.end(); it++)
    {
        if ((*it)->getSStart() == time) break;
        row++;
    }
    if (row == subtitles.size()) return -1;
    return row;
}

void Subtitles::addSubTitle(Subtitle *sub)
{
    subtitles.push_back(sub);
    if (subtitles.size() > 1 && sub->getStart() <
            subtitles[subtitles.size() - 2]->getStart())
                std::sort(subtitles.begin(), subtitles.end(),
                          [](const Subtitle *a, const Subtitle *b) -> bool {
                    return a->getStart() < b->getStart();
                });
    edited = true;
}

