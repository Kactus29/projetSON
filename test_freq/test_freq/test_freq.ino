#include <Audio.h>

// Créer des objets pour la capture audio et l'analyse de fréquence
AudioInputI2S            i2s1;           // Entrée audio via I2S
AudioAnalyzeNoteFrequency notefreq;      // Analyseur de fréquence de note
AudioConnection          patchCord1(i2s1, 0, notefreq, 0); // Connexion entre l'entrée audio et l'analyseur de fréquence

void setup() {
  // Initialiser la communication série
  Serial.begin(9600);

  // Initialiser la mémoire audio
  AudioMemory(12);

  // Démarrer l'analyseur de fréquence avec un seuil de 0.15
  notefreq.begin(0.15);
}

void loop() {
  // Vérifier si une nouvelle fréquence est disponible
  if (notefreq.available()) {
    // Lire la fréquence détectée
    float frequency = notefreq.read();
    float probability = notefreq.probability();

    // Afficher la fréquence et la probabilité sur la console série
    Serial.print("Fréquence détectée : ");
    Serial.print(frequency);
    Serial.print(" Hz | Probabilité : ");
    Serial.println(probability);
  }
}
