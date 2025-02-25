#ifndef STORE_MELODY_H
#define STORE_MELODY_H

#include "include.h"

void initStorage();
void storeMelody(const std::vector<float>& melody, const char* path, const char* filename);
std::vector<float> loadMelody(const char* path, const char* filename);
std::vector<String> getStoredMelodies(const char* path);

#endif // STORE_MELODY_H
