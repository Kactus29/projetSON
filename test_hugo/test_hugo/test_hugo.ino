#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// Déclaration des objets audio
AudioInputI2S in;                   // Entrée micro I2S
AudioAnalyzeNoteFrequency notefreq;  // Analyseur de fréquence
AudioAnalyzePeak peak;

// Connexion entre l'entrée audio et l'analyseur
AudioConnection patchCord(in, 0, notefreq, 0);
AudioConnection patchCord2(in, 0, peak, 0);

void setup() {
    Serial.begin(115200); // Démarrer la communication série
    AudioMemory(30);      // Allouer de la mémoire pour le traitement audio
    notefreq.begin(0.15); // Définir le seuil de détection (plus bas = plus sensible)
    AudioControlSGTL5000 audioShield;
    audioShield.enable();
    audioShield.inputSelect(AUDIO_INPUT_MIC); // Sélection du micro
    audioShield.micGain(2); // Augmente le gain du micro pour capter plus de son

}

void loop() {
    // Serial.println("Fuck");
    // if (peak.available()) {
    //     float amp = peak.read();
    //     Serial.println(amp);
    // }
    if (notefreq.available()) {
        float freq = notefreq.read(); 
        float prob = notefreq.probability(); 
        Serial.println(freq);
        Serial.println(prob);
        Serial.println("\n");
    }
    delay(10); // Limiter le nombre d'affichages par seconde
}

