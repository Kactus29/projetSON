#include <Arduino.h>
#include "CaptureAudio.h"

IntervalTimer captureTimer;
bool capturing = false;

void setup() {
  // Initialiser la communication série
  Serial.begin(9600);
  // Initialiser le microphone
  initMicrophone();
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 's') { // start
      // Commencer la capture des notes
      capturing = true;
      captureTimer.begin(captureNote, 10 * 1000); // 10 ms
      Serial.println("Capture commencée.");
    } else if (command == 'e') {  // end
      // Arrêter la capture des notes
      capturing = false;
      captureTimer.end();
      Serial.println("Capture terminée.");
      // Afficher les notes capturées
      std::vector<float> notes = getCapturedNotes();
      for (float note : notes) {
        Serial.println(note);
      }
    }
  }
}
