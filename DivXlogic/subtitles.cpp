#include "subtitles.h"

Subtitles::Subtitles() {}

Subtitles::Subtitles(int f) : FPS(f) {}

double Subtitles::getFPS() const { return FPS; }

long Subtitles::subtitleCo() const { return subtitles.size(); }

vector<Subtitle*> &Subtitles::getTitles() { return subtitles; }

vector<Subtitle*> const &Subtitles::getTitles() const
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
