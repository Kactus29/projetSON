#ifndef STORE_MELODY_H
#define STORE_MELODY_H

#include <SD.h>
#include <SPI.h>
#include <String.h>
#include <vector>
#include <Arduino.h>

void initStorage();
void storeMelody(const std::vector<float>& melody, const char* directory, const char* filename);
std::vector<float> loadMelody(const char* directory, const char* filename);
std::vector<String> getStoredMelodies(const char* directory);

#endif // STORE_MELODY_H
