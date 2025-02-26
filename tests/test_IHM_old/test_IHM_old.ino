#include "include.h"

void setup() {
  // Initialiser le stockage
  setup();
  // Initialiser le microphone
  initMicrophone();
  // Afficher les commandes
  displayTools();
}

void loop() {
  handleCommand();
}