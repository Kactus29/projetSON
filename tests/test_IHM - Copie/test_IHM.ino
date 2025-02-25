#include "include.h"

void setup() {
  // Initialiser le stockage
  initStorage();
  // Initialiser le microphone
  initMicrophone();
}

void loop() {
  handleCommand();
}