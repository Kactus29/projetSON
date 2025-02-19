#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//---------------------------------------------------------------------------------------
#include "guitar_e2_note.h"
#include "guitar_a2_note.h"
#include "guitar_d3_note.h"
#include "guitar_g3_note.h"
#include "guitar_b3_note.h"
#include "guitar_e4_note.h"
#include "tuba_1.h"
#include "tuba_2.h"
#include "tuba_3.h"
#include "tuba_4.h"
#include "tuba_5.h"
//--------------------------------------------------------------------------------------- 
// Créer des objets pour la capture audio et l'analyse de fréquence
AudioInputI2S             in;                             // Entrée audio via I2S
AudioOutputAnalog         dac;                            // Sortie audio via casque
AudioPlayMemory           wav_note;                       // Empreinte = dernière note jouée
AudioMixer4               mixer;                          // Mélangeur audio (des examples)
AudioAnalyzeNoteFrequency notefreq;                       // Analyseur de fréquence de note
AudioAnalyzePeak          peak;                           // Analyseur d'amplitude de note
AudioAmplifier            amp;                            // Amplificateur audio
//---------------------------------------------------------------------------------------
AudioConnection patchCord1(in, 0, amp, 0);                // Connexion entre l'entrée audio et l'amplificateur
AudioConnection patchCord2(amp, 0, notefreq, 0);          // Connexion entre l'amplificateur et l'analyseur de fréquence
AudioConnection patchCord3(wav_note, 0, mixer, 0);        // Connexion entre l'instrument simulé et le mélangeur
AudioConnection patchCorwd4(mixer, 0, dac, 0);             // Connexion entre le mélangeur et le casque audio
AudioConnection patchCord5(mixer, 0, notefreq, 0);        // Connexion entre le mélangeur et l'analyseur de fréquence
AudioConnection patchCord6(in, 0, peak, 0);               // Connexion entre l'entrée audio et l'analyseur d'amplitude
//---------------------------------------------------------------------------------------
IntervalTimer playNoteTimer;

void playNote(void) {
  if (!wav_note.isPlaying()) {
    // Uncomment one of these sounds to test notefreq
    wav_note.play(guitar_e2_note);
    // wav_note.play(guitar_a2_note);
    // wav_note.play(guitar_d3_note);
    // wav_note.play(guitar_g3_note);
    // wav_note.play(guitar_b3_note);
    // wav_note.play(guitar_e4_note);
    // wav_note.play(tuba_1);
    // wav_note.play(tuba_2);
    // wav_note.play(tuba_3);
    // wav_note.play(tuba_4);
    // wav_note.play(tuba_5);
    digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
  }
}

void setup() {
  // Initialiser la communication série
  Serial.begin(9600);
  // Initialiser la mémoire audio
  AudioMemory(30);
  // Démarrer l'analyseur de fréquence avec un seuil de 0.15
  notefreq.begin(0.15);
  // Configurer l'amplificateur avec un gain de 5.0
  amp.gain(5.0);
  pinMode(LED_BUILTIN, OUTPUT);
  playNoteTimer.priority(144);
  playNoteTimer.begin(playNote, 1000);
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
  } else {
    // Afficher un message si aucune fréquence n'est disponible
    Serial.println("Aucune fréquence détectée.");
  }
  // Ajouter un délai pour éviter de saturer la console série
  delay(100);
}