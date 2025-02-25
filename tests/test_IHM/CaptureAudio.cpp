#include "include.h"

//--------------------------------------------------------------------------------------- 
AudioInputI2S             in;           // Entrée audio via I2S
AudioControlSGTL5000      audioShield;  // Gestion de la saturation
AudioAmplifier            amp;          // Amplificateur audio
AudioAnalyzeNoteFrequency notefreq;     // Analyseur de fréquence de note
//---------------------------------------------------------------------------------------
AudioConnection patchCord1(in, 0, amp, 0);        // Connexion entre l'entrée audio et l'amplificateur
AudioConnection patchCord2(amp, 0, notefreq, 0);  // Connexion entre l'amplificateur et l'analyseur de fréquence

float lastFrequency = 0.0; // Variable to store the last detected frequency

/**
 * @brief Initialise le microphone et les composants audio.
 * 
 * Configure la mémoire audio, démarre l'analyseur de fréquence avec un seuil de 0.15
 * et configure l'amplificateur avec un gain de 5.0.
 */
void initMicrophone() {
  AudioMemory(30);
  notefreq.begin(0.15);
  amp.gain(5.0);
}

/**
 * @brief Capture une note jouée et l'ajoute à la liste des notes capturées.
 * 
 * Vérifie si une nouvelle fréquence est disponible, lit la fréquence détectée et sa probabilité associée,
 * puis ajoute la fréquence capturée à la liste des notes capturées. Affiche également la fréquence et la probabilité sur la console série.
 * Si aucune nouvelle fréquence n'est détectée dans les 10 ms, stocke 0.
 */
void captureNote() {
  if (notefreq.available()) {
    lastFrequency = notefreq.read();
    float probability = notefreq.probability();
    Serial.print("Fréquence détectée : ");
    Serial.print(lastFrequency);
    Serial.print(" Hz | Probabilité : ");
    Serial.println(probability);
  } else {
    lastFrequency = 0.0; // No new frequency detected, set to 0
  }
  capturedNotes.push_back(lastFrequency);
}

/**
 * @brief Retourne la liste des notes capturées.
 * 
 * @return std::vector<float> Liste des fréquences des notes capturées.
 */
std::vector<float> getCapturedNotes() {
  return capturedNotes;
}