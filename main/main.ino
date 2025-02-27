#include "include.h"

void setup() {
  // Initialiser le stockage
  initStorage();
  // Initialiser le microphone
  initMicrophone();
  // Initialiser la lecture de fichiers WAV
  initOut();
  // Initialiser l'IHM
  initIHM();
}

void loop() {
  handleCommand();
}