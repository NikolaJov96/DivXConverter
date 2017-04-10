#include "subtitles.h"

Subtitles::Subtitles() {}

Subtitles::Subtitles(int f) : FPS(f) {}

int Subtitles::getFPS() const { return FPS; }

long Subtitles::subtitleCo() const { return subtitles.size(); }

vector<Subtitle*> &Subtitles::getTitles() { return subtitles; }

void Subtitles::setFPS(int f) { FPS = f; }

bool Subtitles::isEmpty() const { return subtitles.size() == 0; }

void Subtitles::addSubTitle(Subtitle *s, long t)
{
    if (t == -1) subtitles.push_back(s);
    else
    {
        // find a place to insert
        // long ind = std::subtitles.
    }
}
