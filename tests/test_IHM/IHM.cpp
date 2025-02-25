#include "include.h"

void handleCommand() {
  char filename[64] = "some_file";
  char tempPath[512];

  if (Serial.available()) {
    char command = Serial.read();

    if (command == 's') { // start capture
      capturing = true;
      capturedNotes.clear();
      captureTimer.begin(captureNote, 10 * 1000); // 10 ms
      Serial.println("Capture commencée.");
      
    } else if (command == 'e') {  // end capture
      capturing = false;
      captureTimer.end();
      Serial.println("Capture terminée.");
      std::vector<float> notes = getCapturedNotes();
      for (float note : notes) {
        Serial.println(note);
      }

    } else if (command == 'w') {  // write melody to SD
      Serial.println("Entrez le nom du répertoire (enr ou trad) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', path, sizeof(path));
      path[sizeof(path) - 1] = '\0';
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "/%s", path);
      storeMelody(capturedNotes, tempPath, filename);

    } else if (command == 'r') { // read melody from SD
      Serial.println("Entrez le nom du répertoire (enr ou trad) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', path, sizeof(path));
      path[sizeof(path) - 1] = '\0';
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "/%s", path);
      std::vector<float> melody = loadMelody(tempPath, filename);
      for (float note : melody) {
        Serial.println(note);
      }

    } else if (command == 'l') { // list stored melodies
      Serial.println("Entrez le nom du répertoire (enr ou trad) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', path, sizeof(path));
      path[sizeof(path) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "/%s", path);
      std::vector<String> melodies = getStoredMelodies(tempPath);
      for (String melody : melodies) {
        Serial.println(melody);
      }

    } else if (command == 'c') { // compare music
      Serial.println("Entrez le nom du répertoire (enr ou trad) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', path, sizeof(path));
      path[sizeof(path) - 1] = '\0';
      Serial.println("Entrez le nom du fichier cible (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "/%s", path);
      std::vector<std::pair<String, float>> bestMatch = findMatchingMelody(loadMelody(tempPath, filename));
      Serial.println("La mélodie la plus similaire est : ");
      for (const auto& match : bestMatch) {
        Serial.print("Fichier: ");
        Serial.print(match.first);
        Serial.print(" | Score: ");
        Serial.println(match.second);
      }

    } else if (command == 'd') { // delete file
      Serial.println("Entrez le nom du répertoire (enr ou trad) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', path, sizeof(path));
      path[sizeof(path) - 1] = '\0';
      Serial.println("Entrez le nom du fichier à supprimer (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "/%s/%s", path, filename);
      if (SD.remove(tempPath)) {
        Serial.println("Fichier supprimé avec succès.");
      } else {
        Serial.println("Erreur lors de la suppression du fichier.");
      }
    }
  }
}