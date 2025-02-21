#ifndef COMPARE_MUSIC_H
#define COMPARE_MUSIC_H

#include <vector>
#include <String.h>

bool compareMelodies(const std::vector<float>& capturedNotes, const std::vector<float>& storedMelody);
String findMatchingMelody(const std::vector<float>& capturedNotes);
float calculateCorrelation(const std::vector<float>& a, const std::vector<float>& b);

#endif // COMPARE_MUSIC_H