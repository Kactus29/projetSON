#include "StoreMelody.h"
#include <SD.h>
#include <SPI.h>

/**
 * @brief Initialise le stockage sur la carte SD.
 * 
 * Vérifie si la carte SD est correctement initialisée. Affiche un message d'erreur en cas d'échec.
 */
void initStorage() {
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Erreur d'initialisation de la carte SD");
    return;
  }
  Serial.println("Carte SD initialisée.");
}

/**
 * @brief Stocke une mélodie dans un fichier sur la carte SD.
 * 
 * @param melody Vecteur contenant les fréquences des notes de la mélodie.
 * @param filename Nom du fichier dans lequel stocker la mélodie.
 */
void storeMelody(const std::vector<float>& melody, const char* filename) {
  File file = SD.open(filename, FILE_WRITE);
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
 * @param filename Nom du fichier à partir duquel charger la mélodie.
 * @return std::vector<float> Vecteur contenant les fréquences des notes de la mélodie chargée.
 */
std::vector<float> loadMelody(const char* filename) {
  std::vector<float> melody;
  File file = SD.open(filename);
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
 * @brief Retourne la liste des mélodies stockées sur la carte SD.
 * 
 * @return std::vector<String> Vecteur contenant les noms des fichiers des mélodies stockées.
 */
std::vector<String> getStoredMelodies() {
  std::vector<String> melodies;
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
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