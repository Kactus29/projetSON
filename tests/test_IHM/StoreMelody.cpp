#include "include.h"

#define SDCARD_CS_PIN 10

/**
 * @brief Initialise le stockage sur la carte SD.
 * 
 * Vérifie si la carte SD est correctement initialisée. Affiche un message d'erreur en cas d'échec.
 */
void initStorage() {
  while (!Serial) {
    delay(10); // Attendre que le port série soit prêt
  }
  Serial.begin(9600);
  Serial.println("------------");
  Serial.println("Initialisation de la carte SD...");
  Serial.println("------------");
  if (!SD.begin(SDCARD_CS_PIN)) {
    Serial.println("Échec de l'initialisation !");
    return;
  }
  Serial.println("Initialisation réussie.");
}

/**
 * @brief Stocke une mélodie dans un fichier sur la carte SD.
 * 
 * @param melody Vecteur contenant les fréquences des notes de la mélodie.
 * @param path Chemin du répertoire dans lequel stocker la mélodie.
 * @param filename Nom du fichier dans lequel stocker la mélodie.
 */
void storeMelody(const std::vector<float>& melody, const char* path, const char* filename) {
  String fullPath = String("/") + String(path) + "/" + filename;
  File file = SD.open(fullPath.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Erreur d'ouverture du fichier pour l'écriture");
    return;
  }
  for (float note : melody) {
    file.println(note);
  }
  file.close();
  Serial.println("Mélodie stockée avec succès.");
}

/**
 * @brief Charge une mélodie depuis un fichier sur la carte SD.
 * 
 * @param path Chemin du répertoire à partir duquel charger la mélodie.
 * @param filename Nom du fichier à partir duquel charger la mélodie.
 * @return std::vector<float> Vecteur contenant les fréquences des notes de la mélodie chargée.
 */
std::vector<float> loadMelody(const char* path, const char* filename) {
  std::vector<float> melody;
  String fullPath = String("/") + String(path) + "/" + filename;
  File file = SD.open(fullPath.c_str());
  if (!file) {
    Serial.println("Erreur d'ouverture du fichier pour la lecture");
    return melody;
  }
  while (file.available()) {
    melody.push_back(file.parseFloat());
  }
  file.close();
  Serial.println("Mélodie chargée avec succès.");
  return melody;
}

/**
 * @brief Retourne la liste des mélodies stockées dans un répertoire sur la carte SD.
 * 
 * @param path Chemin du répertoire à partir duquel lister les mélodies stockées.
 * @return std::vector<String> Vecteur contenant les noms des fichiers des mélodies stockées.
 */
std::vector<String> getStoredMelodies(const char* path) {
  std::vector<String> melodies;
  String fullPath = String("/") + String(path);
  File dir = SD.open(fullPath.c_str());
  if (!dir) {
    Serial.println("Erreur d'ouverture du répertoire");
    return melodies;
  }
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }
    if (!entry.isDirectory()) {
      melodies.push_back(entry.name());
    }
    entry.close();
  }
  return melodies;
}

/**
 * @brief Retourne la liste des dossiers stockés dans un répertoire sur la carte SD.
 * 
 * @param path Chemin du répertoire à partir duquel lister les dossiers stockés.
 * @return std::vector<String> Vecteur contenant les noms des dossiers stockés.
 */
std::vector<String> getStoredDirectories(const char* path) {
  std::vector<String> directories;
  String fullPath = String("/") + String(path);
  File dir = SD.open(fullPath.c_str());
  if (!dir) {
    Serial.println("Erreur d'ouverture du répertoire");
    return directories;
  }
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }
    if (entry.isDirectory()) {
      directories.push_back(entry.name());
    }
    entry.close();
  }
  return directories;
}
