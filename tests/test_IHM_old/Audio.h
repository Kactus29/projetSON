#ifndef AUDIO_H
#define AUDIO_H

#include "include.h"

void initMicrophone();
void captureNote();
std::vector<float> getCapturedNotes();

void initOut();
void playWavFile(const char* filename);

#endif // AUDIO_H