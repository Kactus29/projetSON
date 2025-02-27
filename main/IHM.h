#ifndef IHM_H
#define IHM_H

#include "include.h"

void initIHM();
void handleCommand();
void startCapture();
void stopCapture();
void saveMelody();
void loadMelody();
void listStoredMelodies();
void compareMelody();
void deleteFile();
void changeDirectory();
void moveUpDirectory();
void playMelody();
void displayTools();

#endif // IHM_H