#include "include.h"

void setup() {
  // Initialiser le stockage
  setup();
  // Initialiser le microphone
  initMicrophone();
  // Initialiser le lecteur WAV
  initWavPlayer();
  // Afficher les commandes
  displayTools();
}

void loop() {
  handleCommand();
}