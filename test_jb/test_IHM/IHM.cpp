#include "include.h"
#include "melody.h"

extern IntervalTimer captureTimer;
extern bool capturing;
extern char filename[32];
extern char directory[32];

void handleCommand() {
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
      Serial.readBytesUntil('\n', directory, sizeof(directory));
      directory[sizeof(directory) - 1] = '\0';
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      storeMelody(capturedNotes, directory, filename);

    } else if (command == 'r') { // read melody from SD
      Serial.println("Entrez le nom du répertoire (enr ou trad) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', directory, sizeof(directory));
      directory[sizeof(directory) - 1] = '\0';
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      std::vector<float> melody = loadMelody(directory, filename);
      for (float note : melody) {
        Serial.println(note);
      }

    } else if (command == 'l') { // list stored melodies
      Serial.println("Entrez le nom du répertoire (enr ou trad) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', directory, sizeof(directory));
      directory[sizeof(directory) - 1] = '\0';
      std::vector<String> melodies = getStoredMelodies(directory);
      for (String melody : melodies) {
        Serial.println(melody);
      }

    } else if (command == 'c') { // compare music
      Serial.println("Entrez le nom du répertoire (enr ou trad) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', directory, sizeof(directory));
      directory[sizeof(directory) - 1] = '\0';
      Serial.println("Entrez le nom du fichier cible (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      std::vector<std::pair<String, float>> bestMatch = findMatchingMelody(loadMelody(directory, filename));
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
      Serial.readBytesUntil('\n', directory, sizeof(directory));
      directory[sizeof(directory) - 1] = '\0';
      Serial.println("Entrez le nom du fichier à supprimer (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      String path = String(directory) + "/" + filename;
      if (SD.remove(path.c_str())) {
        Serial.println("Fichier supprimé avec succès.");
      } else {
        Serial.println("Erreur lors de la suppression du fichier.");
      }
    }
  }
}