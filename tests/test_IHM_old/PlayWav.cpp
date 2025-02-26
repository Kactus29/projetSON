#include "include.h"

//---------------------------------------------------------------------------------------
AudioPlaySdWav           playWav;       // Player for WAV files
AudioOutputI2S           out;           // Output audio via I2S
AudioConnection          patchCord3(playWav, 0, out, 0);
AudioConnection          patchCord4(playWav, 1, out, 1);
//---------------------------------------------------------------------------------------

/**
 * @brief Initialise le lecteur de fichiers WAV.
 * 
 * Configure la mémoire audio et initialise le lecteur de fichiers WAV.
 */
void initWavPlayer() {
  AudioMemory(30);
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Erreur d'initialisation de la carte SD");
    return;
  }
  Serial.println("Lecteur WAV initialisé.");
}

/**
 * @brief Joue un fichier WAV depuis la carte SD.
 * 
 * @param filename Nom du fichier WAV à jouer.
 */
void playWavFile(const char* filename) {
  char filepath[256];
  snprintf(filepath, sizeof(filepath), "wav/%s", filename);
  if (playWav.isPlaying()) {
    playWav.stop();
  }
  playWav.play(filepath);
  Serial.print("Lecture du fichier : ");
  Serial.println(filepath);
}
