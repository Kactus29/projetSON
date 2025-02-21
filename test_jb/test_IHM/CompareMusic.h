#ifndef COMPARE_MUSIC_H
#define COMPARE_MUSIC_H

#include "include.h"

bool compareMelodies(const std::vector<float>& capturedNotes, const std::vector<float>& storedMelody);
String findMatchingMelody(const std::vector<float>& capturedNotes);

#endif // COMPARE_MUSIC_H