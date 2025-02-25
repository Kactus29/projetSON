#include "include.h"

unsigned long startTime = 0; // Variable to store le start time

void handleCommand() {
  char filename[64] = "";
  char tempPath[512];
  String currentPath = "/";

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "record") { // start capture
      capturing = true;
      capturedNotes.clear();
      startTime = millis();
      captureTimer.begin(captureNote, 100 * 1000); // 100 ms
      Serial.println("--------------------");
      Serial.println("Capture commencée.");
      Serial.println("--------------------");
      
    } else if (command == "stop") {  // end capture
      capturing = false;
      captureTimer.end();
      Serial.println("--------------------");
      Serial.println("Capture terminée.");
      Serial.println("--------------------");
      std::vector<float> notes = getCapturedNotes();
      for (float note : notes) {
        Serial.println(note);
      }

    } else if (command == "save") {  // write melody to SD
      Serial.println("--------------------");
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      Serial.println("--------------------");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "%s/%s", currentPath.c_str(), filename);
      storeMelody(capturedNotes, currentPath.c_str(), filename);

    } else if (command == "load") { // read melody from SD
      Serial.println("--------------------");
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      Serial.println("--------------------");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "%s/%s", currentPath.c_str(), filename);
      std::vector<float> melody = loadMelody(currentPath.c_str(), filename);
      for (float note : melody) {
        Serial.println(note);
      }

    } else if (command == "ls") { // list stored melodies and directories
      Serial.println("--------------------");
      Serial.println("Chemin actuel :");
      Serial.println(currentPath);
      Serial.println("--------------------");
      Serial.println("Mélodies stockées :");
      std::vector<String> melodies = getStoredMelodies(currentPath.c_str());
      for (String melody : melodies) {
        Serial.println(melody);
      }
      Serial.println("--------------------");
      Serial.println("Dossiers disponibles :");
      std::vector<String> directories = getStoredDirectories(currentPath.c_str());
      for (String directory : directories) {
        Serial.println(directory);
      }
      Serial.println("--------------------");

    } else if (command == "compare") { // compare music
      Serial.println("Entrez le nom du fichier cible (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "%s/%s", currentPath.c_str(), filename);
      std::vector<std::pair<String, float>> bestMatch = findMatchingMelody(loadMelody(currentPath.c_str(), filename));
      Serial.println("La mélodie la plus similaire est : ");
      for (const auto& match : bestMatch) {
        Serial.print("Fichier: ");
        Serial.print(match.first);
        Serial.print(" | Score: ");
        Serial.println(match.second);
      }

    } else if (command == "delete") { // delete file
      Serial.println("Entrez le nom du fichier à supprimer (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "%s/%s", currentPath.c_str(), filename);
      if (SD.remove(tempPath)) {
        Serial.println("Fichier supprimé avec succès.");
      } else {
        Serial.println("Erreur lors de la suppression du fichier.");
      }

    } else if (command == "cd") { // change directory
      Serial.println("Entrez le nom du répertoire :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', path, sizeof(path));
      path[sizeof(path) - 1] = '\0';
      String newPath = currentPath;
      if (newPath != "/") {
        newPath += "/";
      }
      newPath += path;
      File dir = SD.open(newPath.c_str());
      if (dir && dir.isDirectory()) {
        currentPath = newPath;
        Serial.print("Chemin actuel: ");
        Serial.println(currentPath);
      } else {
        Serial.println("Le répertoire n'existe pas.");
      }
      dir.close();

    } else if (command == "up") { // move up one directory
      int lastSlash = currentPath.lastIndexOf('/');
      if (lastSlash != -1) {
        currentPath = currentPath.substring(0, lastSlash);
      }
      if (currentPath == "") {
        currentPath = "/";
      }
      Serial.print("Chemin actuel: ");
      Serial.println(currentPath);
    }
  }
}