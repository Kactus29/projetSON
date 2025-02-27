#include "include.h"

//--------------------------------------------------------------------------------------- 
AudioInputI2S             in;           // Entrée audio via I2S
AudioControlSGTL5000      audioShield;  // Gestion de la saturation
AudioAmplifier            amp;          // Amplificateur audio
AudioAnalyzeNoteFrequency notefreq;     // Analyseur de fréquence de note
AudioConnection patchCord1(in, 0, amp, 0);        // Connexion entre l'entrée audio et l'amplificateur
AudioConnection patchCord2(amp, 0, notefreq, 0);  // Connexion entre l'amplificateur et l'analyseur de fréquence
float lastFrequency = 0.0; // Variable to store the last detected frequency
extern unsigned long startTime; // Variable to store the start time of the melody
//---------------------------------------------------------------------------------------
AudioPlaySdWav           playWav;       // Player for WAV files
AudioOutputI2S           out;           // Output to I2S
AudioConnection          patchCord3(playWav, 0, out, 0);
AudioConnection          patchCord4(playWav, 1, out, 1);
float                    volume = 0.5;  // Initial volume
//---------------------------------------------------------------------------------------

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
  
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(20); // Augmenter le gain du microphone
  audioShield.volume(1.0);
}

/**
 * @brief Capture une note jouée et l'ajoute à la liste des notes capturées.
 * 
 * Vérifie si une nouvelle fréquence est disponible, lit la fréquence détectée et sa probabilité associée,
 * puis ajoute la fréquence capturée à la liste des notes capturées. Affiche également la fréquence et la probabilité sur la console série.
 * Si aucune nouvelle fréquence n'est détectée dans les 10 ms, stocke 0.
 */
 void captureNote() {
  unsigned long currentTime = millis(); // Get the current time
  unsigned long elapsedTime = currentTime - startTime; // Calculate elapsed time

  if (notefreq.available()) {
    lastFrequency = notefreq.read();
    float probability = notefreq.probability();
    Serial.print("Temps écoulé : ");
    Serial.print(elapsedTime);
    Serial.print(" ms | Fréquence détectée : ");
    Serial.print(lastFrequency);
    Serial.print(" Hz | Probabilité : ");
    Serial.println(probability);
  } else {
    lastFrequency = 0.0; // No new frequency detected, set to 0
    Serial.print("Temps écoulé : ");
    Serial.print(elapsedTime);
    Serial.print(" ms | Fréquence détectée : ");
    Serial.print(lastFrequency);
    Serial.println(" Hz | Probabilité : 0.0");
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

/**
 * @brief Initialise la lecture de fichiers WAV depuis la carte SD.
 * 
 * Configure les broches du bouton et du potentiomètre, puis initialise la lecture de fichiers WAV.
 */
void initOut() {
  pinMode(1, INPUT_PULLUP);}

/**
 * @brief Joue un fichier WAV depuis le répertoire courant.
 * 
 * @param filename Nom du fichier WAV à jouer.
 */
void playWavFile(const char* filename) {
  if (!SD.exists(filename)) {
    Serial.print("Erreur : Le fichier ");
    Serial.print(filename);
    Serial.println(" n'existe pas.");
    return;
  }
  Serial.print("Lecture du fichier : ");
  Serial.println(filename);
  if (playWav.isPlaying()) {
    playWav.stop();
  }
  playWav.play(filename);
  
  while (playWav.isPlaying()) {
    int SensorValue = analogRead(A0);
    // Serial.println(SensorValue);
    volume = SensorValue / 1023.0;
    // Serial.println(volume);
    audioShield.volume(volume);
    
    if (digitalRead(1)) {
      playWav.stop();
      Serial.println("Lecture arrêtée par l'utilisateur.");
      break;
    }
    
    delay(100);
  }
  
  Serial.print("Fin de lecture du fichier : ");
  Serial.println(filename);
}