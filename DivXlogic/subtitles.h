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

    double getFPS() const;
    vector<Subtitle*> &getTitles();
    vector<Subtitle*> const &getTitles() const;
    void setFPS(double);

    long subtitleCo() const;
    bool isEmpty() const;
    void addSubTitle(Subtitle*, long t = -1);
private:
    vector<Subtitle*> subtitles;
    double FPS = 25.0;
};

#endif // SUBTITLES_H
