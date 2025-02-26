#include "include.h"

//---------------------------------------------------------------------------------------
AudioPlaySdWav           playWav;       // Player for WAV files
AudioOutputI2S           out;           // Output to I2S
AudioConnection          patchCord3(playWav, 0, out, 0);
AudioConnection          patchCord4(playWav, 1, out, 1);
//---------------------------------------------------------------------------------------

/**
 * @brief Joue un fichier WAV depuis le répertoire courant.
 * 
 * @param filename Nom du fichier WAV à jouer.
 */
void playWavFile(const char* filename) {
  Serial.print("Lecture du fichier : ");
  Serial.println(filename);
  if (playWav.isPlaying()) {
    playWav.stop();
  }
  playWav.play(filename);
  
  // Wait until the WAV file finishes playing
  while (playWav.isPlaying()) {
    delay(100); // Small delay to avoid busy-waiting
  }
  
  Serial.print("Fin de lecture du fichier : ");
  Serial.println(filename);
}
