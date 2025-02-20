#include "StoreMelody.h"
#include <SD.h>
#include <SPI.h>
//--------------------------------------------------------------------------------------- 
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7   // Teensy 4 ignores this, uses pin 11
#define SDCARD_SCK_PIN   14  // Teensy 4 ignores this, uses pin 13
//--------------------------------------------------------------------------------------- 
/**
 * @brief Initialise le stockage sur la carte SD.
 * 
 * Vérifie si la carte SD est correctement initialisée. Affiche un message d'erreur en cas d'échec.
 */
void initStorage() {
  Sd2Card card;
  File melodyFile;
  char buffer[512];
  boolean status;

  while (!Serial) {
    delay(10); // Attendre que le port série soit prêt
  }

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);

  Serial.begin(9600);
  Serial.println("------------");
  Serial.println("SD Card Test");
  Serial.println("------------");

  status = card.init(SPI_FULL_SPEED, SDCARD_CS_PIN);
  if (status) {
    Serial.println("SD card is connected :-)");
  } else {
    Serial.println("SD card is not connected or unusable :-(");
    return;
  }

  Serial.println("------------");
  Serial.print("Initialisation de la carte SD...");
  Serial.println("------------");

  status = SD.begin(SDCARD_CS_PIN);
  if (status) {
    Serial.println("SD library is able to access the filesystem");
  } else {
    Serial.println("SD library can not access the filesystem!");
    Serial.println("Please report this problem, with the make & model of your SD card.");
    Serial.println("  http://forum.pjrc.com/forums/4-Suggestions-amp-Bug-Reports");
  }

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
