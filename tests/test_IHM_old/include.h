#ifndef INCLUDE_H
#define INCLUDE_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <numeric>
#include <String.h>
#include <vector>

// Declare the variables as extern
extern std::vector<float> capturedNotes;
extern IntervalTimer captureTimer;
extern bool capturing;
extern char filename[128];
extern char directory[128];
extern char path[256];


#include "GestionAudio.h"
#include "StoreMelody.h"
#include "CompareMusic.h"
#include "IHM.h"

#endif // INCLUDE_H