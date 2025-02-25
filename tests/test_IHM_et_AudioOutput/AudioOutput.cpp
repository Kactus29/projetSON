#include "include.h"

//---------------------------------------------------------------------------------------
AudioSynthWaveform       waveform;      // Générateur de forme d'onde
AudioOutputI2S           out;           // Sortie audio via I2S
AudioConnection          patchCord3(waveform, 0, out, 0); // Connexion entre le générateur de forme d'onde et la sortie audio
AudioConnection          patchCord4(waveform, 0, out, 1); // Connexion entre le générateur de forme d'onde et la sortie audio
//---------------------------------------------------------------------------------------

/**
 * @brief Initialise la sortie audio.
 * 
 * Configure la mémoire audio et initialise le générateur de forme d'onde.
 */
void initAudioOutput() {
  AudioMemory(20);
  waveform.begin(WAVEFORM_SINE);
}

/**
 * @brief Joue une mélodie via la sortie audio.
 * 
 * @param melody Vecteur contenant les fréquences des notes de la mélodie.
 */
void playMelody(const std::vector<float>& melody) {
  for (float note : melody) {
    waveform.frequency(note);
    waveform.amplitude(0.25); // Réduire l'amplitude pour diminuer le volume
    delay(100); // Jouer chaque note pendant 100 ms
  }
  waveform.amplitude(0); // Arrêter la sortie audio après avoir joué la mélodie
}
