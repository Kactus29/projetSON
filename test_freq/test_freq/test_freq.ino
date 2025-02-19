#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//---------------------------------------------------------------------------------------
// #include "guitar_e2_note.h"
// #include "guitar_a2_note.h"
// #include "guitar_d3_note.h"
// #include "guitar_g3_note.h"
// #include "guitar_b3_note.h"
// #include "guitar_e4_note.h"
// #include "tuba_1.h"
// #include "tuba_2.h"
// #include "tuba_3.h"
// #include "tuba_4.h"
// #include "tuba_5.h"
//--------------------------------------------------------------------------------------- 
// Créer des objets pour la capture audio et l'analyse de fréquence
AudioInputI2S             in;                             // Entrée audio via I2S
AudioInputI2S             out;                            // Sortie audio via I2S
AudioOutputAnalog         dac;                            // Sortie audio via casque
AudioPlayMemory           wav_note;                       // Empreinte = dernière note jouée
AudioMixer4               mixer;                          // Mélangeur audio (des examples)
AudioAnalyzeNoteFrequency notefreq;                       // Analyseur de fréquence de note
AudioAnalyzePeak          peak;                           // Analyseur d'amplitude de note
//---------------------------------------------------------------------------------------
AudioConnection patchCord(in, 0, notefreq, 0);      // Connexion entre l'entrée audio et l'analyseur de fréquence
AudioConnection patchCord0(wav_note, 0, mixer, 0);  // Connexion entre l'instrument simulé et le mélangeur
AudioConnection patchCord1(mixer, 0, notefreq, 0);  // Connexion entre le mélangeur et l'analyseur de fréquence
AudioConnection patchCord2(mixer, 0, dac, 0);       // Connexion entre le mélangeur et le casque audio
AudioConnection patchCord3(in, 0, peak, 0);       // Connexion entre le mélangeur et le casque audio
//---------------------------------------------------------------------------------------
// IntervalTimer playNoteTimer;

// void playNote(void) {
//     if (!wav_note.isPlaying()) {
//         // Uncomment one of these sounds to test notefreq
//         // wav_note.play(guitar_e2_note);
//         // wav_note.play(guitar_a2_note);
//         // wav_note.play(guitar_d3_note);
//         // wav_note.play(guitar_g3_note);
//         // wav_note.play(guitar_b3_note);
//         // wav_note.play(guitar_e4_note);
//         // wav_note.play(tuba_1);
//         // wav_note.play(tuba_2);
//         // wav_note.play(tuba_3);
//         // wav_note.play(tuba_4);
//         // wav_note.play(tuba_5);
//         digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
//     }
// }

void setup() {
  AudioMemory(30);
  notefreq.begin(.15);
  pinMode(LED_BUILTIN, OUTPUT);
  // playNoteTimer.priority(144);
  // playNoteTimer.begin(playNote, 1000);
}

void loop() {
  // if (peak.available()) {
  if (notefreq.available()) {
    float freq = notefreq.read();
    float amp = peak.read();
    float prob = notefreq.probability();
    Serial.printf("Fréquence: ", freq, " | Amplitude: ", amp, " | Probabilité: ", prob, "\n");
  }
  // }
}
