#ifndef CAPTURE_AUDIO_H
#define CAPTURE_AUDIO_H

#include "include.h"

void initMicrophone();
void captureNote();
std::vector<float> getCapturedNotes();

#endif // CAPTURE_AUDIO_H