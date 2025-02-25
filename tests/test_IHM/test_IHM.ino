#include "include.h"

void setup() {

  // Afficher les commandes disponibles
  displayCommands();

  // Initialiser le stockage
  initStorage();
  // Initialiser le microphone
  initMicrophone();
  // Initialiser la sortie audio
  initAudioOutput();
}

void loop() {
  handleCommand();
}