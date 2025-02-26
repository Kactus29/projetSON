#include "include.h"

void setup() {
  // Initialiser le stockage
  initStorage();
  // Initialiser le microphone
  initMicrophone();
  // Afficher les commandes
  displayTools();
}

void loop() {
  handleCommand();
}