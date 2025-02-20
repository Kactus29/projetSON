#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//--------------------------------------------------------------------------------------- 
// Créer des objets pour la capture audio et l'analyse de fréquence
AudioInputI2S             in;           // Entrée audio via I2S
AudioControlSGTL5000      audioShield;  // Gestion de la saturation
AudioAmplifier            amp;          // Amplificateur audio
AudioAnalyzeNoteFrequency notefreq;     // Analyseur de fréquence de note
//---------------------------------------------------------------------------------------
AudioConnection patchCord1(in, 0, amp, 0);        // Connexion entre l'entrée audio et l'amplificateur
AudioConnection patchCord2(amp, 0, notefreq, 0);  // Connexion entre l'amplificateur et l'analyseur de fréquence
//---------------------------------------------------------------------------------------
std::vector<float> capturedNotes;

// Fonction pour initialiser le microphone
void initMicrophone() {
  // Initialiser la mémoire audio
  AudioMemory(30);
  // Démarrer l'analyseur de fréquence avec un seuil de 0.15
  notefreq.begin(0.15);
  // Configurer l'amplificateur avec un gain de 5.0
  amp.gain(5.0);
}

// Fonction pour capturer une note jouée
void captureNote() {
  // Vérifier si une nouvelle fréquence est disponible
  if (notefreq.available()) {
    // Lire la fréquence détectée et sa probabilité associée
    float frequency = notefreq.read();
    float probability = notefreq.probability();
    // Ajouter la fréquence capturée à la liste
    capturedNotes.push_back(frequency);
    // Afficher la fréquence et la probabilité sur la console série
    Serial.print("Fréquence détectée : ");
    Serial.print(frequency);
    Serial.print(" Hz | Probabilité : ");
    Serial.println(probability);
  } else {
    // Afficher un message si aucune fréquence n'est disponible
    // Serial.println("Aucune fréquence détectée."); ==> trop de spam console
  }
}

// Fonction pour retourner la liste des notes capturées
std::vector<float> getCapturedNotes() {
  return capturedNotes;
}
