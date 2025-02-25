#include "include.h"

void setup() {
  // Initialiser le port série
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // Attendre que le port série soit prêt
  }

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