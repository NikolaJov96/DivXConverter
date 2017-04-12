#include "subtitles.h"

double Subtitles::getFPS() const { return FPS; }

long Subtitles::subtitleCo() const { return (long)subtitles.size(); }

std::vector<Subtitle*> &Subtitles::getTitles() { return subtitles; }

std::vector<Subtitle*> const &Subtitles::getTitles() const
{
    return subtitles;
}

void Subtitles::setFPS(double fps) { FPS = fps; }

bool Subtitles::isEmpty() const { return subtitles.size() == 0; }

void Subtitles::addSubTitle(Subtitle *sub, long t)
{
    if (t == -1) subtitles.push_back(sub);
    else
    {
        // find a place to insert
        // long ind = std::subtitles.
    }
}
