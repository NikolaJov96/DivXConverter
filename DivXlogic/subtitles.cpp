#include "subtitles.h"

Subtitles::Subtitles() {}

Subtitles::Subtitles(int f) : FPS(f) {}

int Subtitles::getFPS() const { return FPS; }

void Subtitles::setFPS(int f) { FPS = f; }
