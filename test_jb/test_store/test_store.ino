#include <Arduino.h>
#include "CaptureAudio.h"
#include "StoreMelody.h"
#include "CompareMusic.h"

IntervalTimer captureTimer;
bool capturing = false;
char filename[32]; // Buffer pour stocker le nom du fichier

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
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0'; // Assurez-vous que le nom du fichier est terminé par un caractère nul
      storeMelody(capturedNotes, filename);
    } else if (command == 'r') { // read melody from SD
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0'; // Assurez-vous que le nom du fichier est terminé par un caractère nul
      std::vector<float> melody = loadMelody(filename);
      for (float note : melody) {
        Serial.println(note);
      }
    } else if (command == 'l') { // list stored melodies
      std::vector<String> melodies = getStoredMelodies();
      for (String melody : melodies) {
        Serial.println(melody);
      }
    } else if (command == 'c') {
      compareMusic("musique.csv");
    }
  }
}
