#include "include.h"

//---------------------------------------------------------------------------------------
AudioPlaySdWav           playWav;       // Player for WAV files
AudioOutputI2S           out;           // Output to I2S
AudioConnection          patchCord3(playWav, 0, out, 0);
AudioConnection          patchCord4(playWav, 1, out, 1);
AudioControlSGTL5000     sgtl5000_1;    // Audio shield
//---------------------------------------------------------------------------------------

void initAudio() {
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
}

/**
 * @brief Joue un fichier WAV depuis le répertoire courant.
 * 
 * @param filename Nom du fichier WAV à jouer.
 */
void playWavFile(const char* filename) {
  if (playWav.isPlaying()) {
    playWav.stop();
  }
  playWav.play(filename);
  Serial.print("Lecture du fichier : ");
  Serial.println(filename);
}
