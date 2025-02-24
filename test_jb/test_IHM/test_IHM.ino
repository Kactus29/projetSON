#include "include.h"

IntervalTimer captureTimer;
bool capturing = false; // Indique si la capture des notes est en cours
char filename[32]; // Buffer pour stocker le nom du fichier

void setup() {
  // Initialiser le stockage
  initStorage();
  // Initialiser le microphone
  initMicrophone();
}

void loop() {
  handleCommand();
}