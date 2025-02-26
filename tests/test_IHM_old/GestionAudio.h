#ifndef GESTION_AUDIO_H
#define GESTION_AUDIO_H

#include "include.h"

void initMicrophone();
void captureNote();
std::vector<float> getCapturedNotes();

void initOut();
void playWavFile(const char* filename);

#endif // GESTION_AUDIO_H