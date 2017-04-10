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
    vector<Subtitle*> &getTitles();
    void setFPS(int);

    long subtitleCo() const;
    bool isEmpty() const;
    void addSubTitle(Subtitle*, long t = -1);
private:
    vector<Subtitle*> subtitles;
    int FPS = 25;
};

#endif // SUBTITLES_H
