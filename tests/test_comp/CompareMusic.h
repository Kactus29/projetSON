#ifndef COMPARE_MUSIC_H
#define COMPARE_MUSIC_H

#include <vector>
#include <Arduino.h>

bool compareMelodies(const std::vector<float>& capturedNotes, const std::vector<float>& storedMelody);
std::vector<std::pair<String, float>> findMatchingMelody(const std::vector<float>& capturedNotes);
float calculateCorrelation(const std::vector<float>& a, const std::vector<float>& b);
float calculateDTW(const std::vector<float>& a, const std::vector<float>& b);

#endif // COMPARE_MUSIC_H