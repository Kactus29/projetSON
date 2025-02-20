#include <Arduino.h>
#include "CaptureAudio.h"
#include "StoreMelody.h"

IntervalTimer captureTimer;
bool capturing = false;

void setup() {
  // Initialiser la communication série
  Serial.begin(9600);
  // Initialiser le microphone
  initMicrophone();
  // Initialiser le stockage
  initStorage();
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 's') { // start capture
      // Commencer la capture des notes
      capturing = true;
      capturedNotes.clear();
      captureTimer.begin(captureNote, 10 * 1000); // 10 ms
      Serial.println("Capture commencée.");
    } else if (command == 'e') {  // end capture
      // Arrêter la capture des notes
      capturing = false;
      captureTimer.end();
      Serial.println("Capture terminée.");
      // Afficher les notes capturées
      std::vector<float> notes = getCapturedNotes();
      for (float note : notes) {
        Serial.println(note);
      }
    } else if (command == 'w') { // write melody to SD
      storeMelody(capturedNotes, "melody.txt");
    } else if (command == 'r') { // read melody from SD
      std::vector<float> melody = loadMelody("melody.txt");
      for (float note : melody) {
        Serial.println(note);
      }
    } else if (command == 'l') { // list stored melodies
      std::vector<String> melodies = getStoredMelodies();
      for (String melody : melodies) {
        Serial.println(melody);
      }
    }
  }
}