#include "include.h"

unsigned long startTime = 0; // Variable to store le start time

// Function to display available commands
void displayCommands() {
  Serial.println("Commandes disponibles :");
  Serial.println("record : Commencer une capture de son");
  Serial.println("stop : Arrêter une capture de son");
  Serial.println("save : Enregistrer la mélodie capturée sur la carte SD");
  Serial.println("load : Charger une mélodie depuis la carte SD");
  Serial.println("ls : Lister les mélodies et répertoires stockés");
  Serial.println("compare : Comparer la mélodie capturée avec celles stockées");
  Serial.println("delete : Supprimer un fichier de la carte SD");
  Serial.println("dir : Changer de répertoire");
  Serial.println("up : Remonter d'un répertoire");
}

void handleCommand() {
  char filename[64] = "";
  char tempPath[512];
  String currentPath = "";

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "record") { // start capture
      capturing = true;
      capturedNotes.clear();
      startTime = millis();
      captureTimer.begin(captureNote, 100 * 1000); // 100 ms
      Serial.println("Capture commencée.");
      
    } else if (command == "stop") {  // end capture
      capturing = false;
      captureTimer.end();
      Serial.println("Capture terminée.");
      std::vector<float> notes = getCapturedNotes();
      for (float note : notes) {
        Serial.println(note);
      }

    } else if (command == "save") {  // write melody to SD
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "%s/%s", currentPath.c_str(), filename);
      storeMelody(capturedNotes, currentPath.c_str(), filename);

    } else if (command == "load") { // read melody from SD
      Serial.println("Entrez le nom du fichier (avec extension .csv) :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', filename, sizeof(filename));
      filename[sizeof(filename) - 1] = '\0';
      snprintf(tempPath, sizeof(tempPath), "%s/%s", currentPath.c_str(), filename);
      std::vector<float> melody = loadMelody(currentPath.c_str(), filename);
      for (float note : melody) {
        Serial.println(note);
      }

    } else if (command == "ls") { // list stored melodies and directories
      Serial.println("Chemin actuel :");
      Serial.println(currentPath);
      Serial.println("Mélodies stockées :");
      std::vector<String> melodies = getStoredMelodies(currentPath.c_str());
      for (String melody : melodies) {
        Serial.println(melody);
      }
      Serial.println("Dossiers disponibles :");
      std::vector<String> directories = getStoredDirectories(currentPath.c_str());
      for (String directory : directories) {
        Serial.println(directory);
      }

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

      if (!bestMatch.empty()) {
        // Trouver la mélodie avec le meilleur score
        auto bestMatchIt = std::max_element(bestMatch.begin(), bestMatch.end(), 
                                            [](const std::pair<String, float>& a, const std::pair<String, float>& b) {
                                              return a.second < b.second;
                                            });
        String bestMatchFile = bestMatchIt->first;
        Serial.print("Meilleure correspondance trouvée : ");
        Serial.println(bestMatchFile);

        std::vector<float> bestMelody = loadMelody(currentPath.c_str(), bestMatchFile.c_str());
        Serial.println("Contenu du fichier :");
        for (float note : bestMelody) {
          Serial.println(note);
        }

        playMelody(bestMelody);
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

    } else if (command == "dir") { // change directory
      Serial.println("Entrez le nom du répertoire :");
      while (!Serial.available()) {}
      Serial.readBytesUntil('\n', path, sizeof(path));
      path[sizeof(path) - 1] = '\0';
      if (currentPath != "") {
        currentPath += "/";
      }
      currentPath += path;
      Serial.print("Chemin actuel: ");
      Serial.println(currentPath);

    } else if (command == "up") { // move up one directory
      int lastSlash = currentPath.lastIndexOf('/');
      if (lastSlash != -1) {
        currentPath = currentPath.substring(0, lastSlash);
      }
      Serial.print("Chemin actuel: ");
      Serial.println(currentPath);
    }
  }
}