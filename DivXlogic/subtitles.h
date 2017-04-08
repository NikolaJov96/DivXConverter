#ifndef SUBTITLES_H
#define SUBTITLES_H

#include <vector>

#include "subtitle.h"

using namespace std;

class Subtitles
{
public:
    Subtitles();
    Subtitles(int);

    int getFPS() const;

    void setFPS(int);
private:
    vector<Subtitle*> subtitles;
    int FPS = 25;
};

#endif // SUBTITLES_H
