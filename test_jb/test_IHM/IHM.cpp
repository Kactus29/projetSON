#include "include.h"

extern IntervalTimer captureTimer;
extern bool capturing;
extern char filename[32];

void handleCommand() {
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

    } else if (command == 'w') {  // write melody to SD
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      storeMelody(capturedNotes, filename);

    } else if (command == 'r') { // read melody from SD
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      std::vector<float> melody = loadMelody(filename);
      for (float note : melody) {
        Serial.println(note);
      }

    } else if (command == 'l') { // list stored melodies
      std::vector<String> melodies = getStoredMelodies();
      for (String melody : melodies) {
        Serial.println(melody);
      }

    } else if (command == 'c') { // compare music
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      std::vector<std::pair<String, float>> bestMatch = findMatchingMelody(loadMelody(filename));
      Serial.println("La mélodie la plus similaire est : ");
      for (const auto& match : bestMatch) {
        Serial.print("Fichier: ");
        Serial.print(match.first);
        Serial.print(" | Score: ");
        Serial.println(match.second);
      }

    } else if (command == 'd') { // delete file
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      if (SD.remove(filename)) {
        Serial.println("Fichier supprimé avec succès.");
      } else {
        Serial.println("Erreur lors de la suppression du fichier.");
      }
    }
  }
}