#ifndef COMPARE_MUSIC_H
#define COMPARE_MUSIC_H

#include "include.h"

std::vector<std::pair<String, float>> findMatchingMelody(const std::vector<float>& capturedNotes);
float calculateCorrelation(const std::vector<float>& a, const std::vector<float>& b);

#endif // COMPARE_MUSIC_H