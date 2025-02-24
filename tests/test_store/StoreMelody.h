#ifndef STORE_MELODY_H
#define STORE_MELODY_H

#include <vector>
#include <Arduino.h>

void initStorage();
void storeMelody(const std::vector<float>& melody, const char* filename);
std::vector<float> loadMelody(const char* filename);
std::vector<String> getStoredMelodies();

#endif // STORE_MELODY_H
