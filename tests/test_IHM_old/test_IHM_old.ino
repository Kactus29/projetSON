#include "include.h"

void setup() {
  // Initialiser le stockage
  initStorage();
  // Initialiser le microphone
  initMicrophone();
  // Initialiser l'audio
  initAudio();
  // Afficher les commandes
  displayTools();
}

void loop() {
  handleCommand();
}