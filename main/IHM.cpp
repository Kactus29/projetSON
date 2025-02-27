#include "include.h"

unsigned long startTime = 0;

void initIHM() {
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  displayTools();
}

void handleCommand() {
  String currentPath = "";
  if (digitalRead(0)) {
    if (!capturing) {
      startCapture();
    } else {
      stopCapture();
    }
  }
  if (digitalRead(1)) {
    playMelody();
  }
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if ((command == "record") || (command == "start") || (command == "capture") || (command == "begin") || (command == "go") || (command == "commence") || (command == "commencer") || (command == "start") || (command == "s") || (command == "rec")) {          
      startCapture();
    } else if ((command == "stop") || (command == "end") || (command == "finish") || (command == "e")) {
      stopCapture();
    } else if ((command == "save") || (command == "store") || (command == "write") || (command == "w")) {
      saveMelody();
    } else if ((command == "cat") || (command == "load") || (command == "read") || (command == "r")) {
      loadMelody();
    } else if ((command == "ls") || (command == "list") || (command == "arch") || (command == "l")) {
      listStoredMelodies();
    } else if ((command == "compare") || (command == "match") || (command == "c")) {
      compareMelody();
    } else if ((command == "delete") || (command == "remove") || (command == "rm") || (command == "d")) {
      deleteFile();
    // } else if ((command == "dir") || (command == "cd")) {
    //   changeDirectory();
    // } else if ((command == "up") || (command == "cd ..")) {
    //   moveUpDirectory();
    } else if ((command == "play") || (command == "p")) {
      playMelody();
    } else if ((command == "help") || (command == "h")) {
      displayTools();
    }
  }
}

void startCapture() {
  capturing = true;
  delay(300);
  capturedNotes.clear();
  startTime = millis();
  captureTimer.begin(captureNote, 100 * 1000); // 100 ms
  Serial.println();
  Serial.println("-----------------------------------------------------");
  Serial.println("Capture commencée.");
  Serial.println("Tapez 'stop' pour arrêter la capture.");
  Serial.println();
}

void stopCapture() {
  capturing = false;
  delay(300);
  captureTimer.end();
  Serial.println("Capture terminée.");
  Serial.println();
  std::vector<float> notes = getCapturedNotes();
  for (float note : notes) {
    Serial.println(note);
  }
}

void saveMelody() {
  char filename[64] = "";
  char tempPath[512];
  String currentPath = "";
  Serial.println();
  Serial.println("-----------------------------------------------------");
  Serial.println("Entrez le nom du fichier (avec extension .csv) :");
  Serial.println();
  while (!Serial.available()) {}
  Serial.readBytesUntil('\n', filename, sizeof(filename));
  filename[sizeof(filename) - 1] = '\0';
  snprintf(tempPath, sizeof(tempPath), "%s/%s", currentPath.c_str(), filename);
  storeMelody(capturedNotes, currentPath.c_str(), filename);
}

void loadMelody() {
  char filename[64] = "";
  char tempPath[512];
  String currentPath = "";
  Serial.println();
  Serial.println("-----------------------------------------------------");
  Serial.println("Entrez le nom du fichier (avec extension .csv) :");
  Serial.println();
  while (!Serial.available()) {}
  Serial.readBytesUntil('\n', filename, sizeof(filename));
  filename[sizeof(filename) - 1] = '\0';
  snprintf(tempPath, sizeof(tempPath), "%s/%s", currentPath.c_str(), filename);
  std::vector<float> melody = loadMelody(currentPath.c_str(), filename);
  for (float note : melody) {
    Serial.println(note);
  }
}

void listStoredMelodies() {
  String currentPath = "";
  Serial.println();
  Serial.println("-----------------------------------------------------");
  // Serial.println("Chemin actuel :"); //debug
  // Serial.println(currentPath); //debug
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
}

void compareMelody() {
  char filename[64] = "";
  char tempPath[512];
  String currentPath = "";
  Serial.println();
  Serial.println("-----------------------------------------------------");
  Serial.println("Entrez le nom du fichier cible (avec extension .csv) :");
  Serial.println();
  while (!Serial.available()) {}
  Serial.readBytesUntil('\n', filename, sizeof(filename));
  filename[sizeof(filename) - 1] = '\0';
  snprintf(tempPath, sizeof(tempPath), "%s/%s", currentPath.c_str(), filename);
  std::vector<std::pair<String, float>> bestMatch = findMatchingMelody(loadMelody(currentPath.c_str(), filename));
  
  // Sort the matches by score in ascending order
  std::sort(bestMatch.begin(), bestMatch.end(), 
            [](const std::pair<String, float>& a, const std::pair<String, float>& b) {
              return a.second < b.second;
            });

  // Affiche toutes les valeurs
  for (const auto& match : bestMatch) {
    Serial.print("Fichier: ");
    Serial.print(match.first);
    Serial.print(" | Score: ");
    Serial.println(match.second);
  }

  // Trouve la meilleure et l'affiche  
  if (!bestMatch.empty()) {
    auto maxElement = bestMatch.back(); // Get the last element after sorting
    String detectedSong = maxElement.first;
    detectedSong.replace(".csv", ""); // Remove the .csv extension
    float maxScore = maxElement.second;
    Serial.print("La chanson détectée est ");
    Serial.print(detectedSong);
    Serial.print(" avec un score de corrélation de ");
    Serial.println(maxScore);

    // Process the detected song name
    if (detectedSong.startsWith("test_")) {
      detectedSong = detectedSong.substring(5); // Remove "test_" prefix
    }
    while (detectedSong.length() > 0 && isDigit(detectedSong.charAt(detectedSong.length() - 1))) {
      detectedSong.remove(detectedSong.length() - 1); // Remove trailing digits
    }

    // Play the detected song
    detectedSong += ".wav"; // Append .wav extension
    playWavFile(detectedSong.c_str());
  } else {
    Serial.println("Aucune correspondance trouvée. Erreur dans le nomage du fichier");
  }
}

void deleteFile() {
  char filename[64] = "";
  char tempPath[512];
  String currentPath = "";
  Serial.println();
  Serial.println("-----------------------------------------------------");
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
}

void changeDirectory() {
  char path[64] = "";
  String currentPath = "";
  Serial.println();
  Serial.println("-----------------------------------------------------");
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
}

void moveUpDirectory() {
  String currentPath = "";
  int lastSlash = currentPath.lastIndexOf('/');
  if (lastSlash != -1) {
    currentPath = currentPath.substring(0, lastSlash);
  }
  Serial.println();
  Serial.println("-----------------------------------------------------");
  Serial.print("Chemin actuel: ");
  Serial.println(currentPath);
}

void playMelody() {
  char filename[64] = "";
  Serial.println();
  Serial.println("-----------------------------------------------------");
  Serial.println("Entrez le nom du fichier WAV (avec extension .wav) :");
  while (!Serial.available()) {}
  Serial.readBytesUntil('\n', filename, sizeof(filename));
  filename[sizeof(filename) - 1] = '\0';
  playWavFile(filename);
}

void displayTools() {
  Serial.println();
  Serial.println("-----------------------------------------------------");
  Serial.println("Commandes disponibles :");
  Serial.println("record - Commence la capture audio");
  Serial.println("stop - Arrête la capture audio");
  Serial.println("save - Sauvegarde la mélodie capturée sur la carte SD");
  Serial.println("cat - Affiche une mélodie depuis la carte SD");
  Serial.println("delete - Supprime un fichier de la carte SD");
  Serial.println("ls - Liste les mélodies et répertoires stockés");
  // Serial.println("dir - Change le répertoire courant");
  // Serial.println("up - Remonte d'un répertoire");
  Serial.println("compare - Compare la mélodie capturée avec celles stockées");
  Serial.println("play - Joue un fichier WAV depuis la carte SD");
  Serial.println("help - Affiche le présent menu");
}