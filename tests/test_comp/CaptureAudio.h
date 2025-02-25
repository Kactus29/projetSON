#ifndef CAPTURE_AUDIO_H
#define CAPTURE_AUDIO_H

#include <vector>
#include <Audio.h>

void initMicrophone();
void captureNote();
std::vector<float> getCapturedNotes();

extern std::vector<float> capturedNotes;

#endif // CAPTURE_AUDIO_H