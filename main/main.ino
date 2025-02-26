#include "include.h"

void setup() {
  // Initialiser le stockage
  initStorage();
  // Initialiser le microphone
  initMicrophone();
  // Initialiser la lecture de fichiers WAV
  initOut();
  // Afficher les commandes
  displayTools();
}

void loop() {
  handleCommand();
}