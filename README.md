# Comparaison de Mélodies sur Teensy

Ce projet vise à comparer des mélodies enregistrées sur une carte Teensy avec celles capturées en temps réel par un microphone. L'objectif est de créer un système simple capable de reconnaître des mélodies de base.

## Table des Matières

1. [Introduction](#introduction)
2. [Modules du Projet](#modules-du-projet)
    - [Module de Capture Audio](#module-de-capture-audio)
    - [Module de Stockage des Mélodies](#module-de-stockage-des-mélodies)
    - [Module de Comparaison de Mélodies](#module-de-comparaison-de-mélodies)
    - [Module de Sortie](#module-de-sortie)
3. [Schéma de Connexion](#schéma-de-connexion)
4. [Exemple de Code](#exemple-de-code)
5. [Conclusion](#conclusion)
6. [Références](#références)

## Introduction

Ce projet propose une structure modulaire pour la comparaison de mélodies sur une carte Teensy. Chaque module peut être développé et testé indépendamment avant d'être intégré dans le projet final. Ce projet peut être étendu avec des fonctionnalités supplémentaires comme la reconnaissance de rythmes ou l'amélioration de l'algorithme de comparaison.

## Modules du Projet

### Module de Capture Audio

- **Fonctionnalité** : Capturer les notes jouées via un microphone.
- **Composants** :
  - Microphone
  - Bibliothèque de traitement audio (ex. : Teensy Audio Library)
- **Fonctions principales** :
  - `initMicrophone()` : Initialiser le microphone.
  - `captureNote()` : Capturer une note jouée.
  - `getCapturedNotes()` : Retourner la liste des notes capturées.

### Module de Stockage des Mélodies

- **Fonctionnalité** : Stocker les mélodies de référence sur la carte Teensy.
- **Composants** :
  - Mémoire EEPROM ou Flash
- **Fonctions principales** :
  - `storeMelody(melody)` : Stocker une mélodie dans la mémoire.
  - `loadMelody(index)` : Charger une mélodie depuis la mémoire.
  - `getStoredMelodies()` : Retourner la liste des mélodies stockées.

### Module de Comparaison de Mélodies

- **Fonctionnalité** : Comparer les notes capturées avec les mélodies stockées.
- **Composants** :
  - Algorithme de comparaison (ex. : Dynamic Time Warping - DTW)
- **Fonctions principales** :
  - `compareMelodies(capturedNotes, storedMelody)` : Comparer les notes capturées avec une mélodie stockée.
  - `findMatchingMelody(capturedNotes)` : Trouver la mélodie stockée qui correspond le mieux aux notes capturées.

### Module de Sortie

- **Fonctionnalité** : Afficher les résultats de la comparaison.
- **Composants** :
  - LEDs ou écran LCD
- **Fonctions principales** :
  - `initOutput()` : Initialiser les composants de sortie.
  - `displayResult(matchFound)` : Afficher le résultat de la comparaison.

## Schéma de Connexion

- Microphone connecté à l'entrée audio de la carte Teensy.
- LEDs ou écran LCD connecté aux broches de sortie de la carte Teensy.

## Exemple de Code

```cpp
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// Module de Capture Audio
AudioInputI2S            i2s1;           // Microphone input
AudioAnalyzeNoteFrequency note1;         // Note frequency analyzer

void initMicrophone() {
  AudioMemory(12);
  i2s1.connectto(0, note1, 0);
}

void captureNote() {
  if (note1.available()) {
    float frequency = note1.read();
    // Ajouter la fréquence capturée à une liste
  }
}

// Module de Stockage des Mélodies
void storeMelody(float melody[]) {
  // Stocker la mélodie dans la mémoire EEPROM ou Flash
}

float* loadMelody(int index) {
  // Charger une mélodie depuis la mémoire
  return melody;
}

// Module de Comparaison de Mélodies
bool compareMelodies(float capturedNotes[], float storedMelody[]) {
  // Comparer les notes capturées avec la mélodie stockée
  return true; // ou false
}

int findMatchingMelody(float capturedNotes[])
```

# ---------- OLD ----------

## projetSON

Projet de SON en arduino

### Quel est ce titre ?

Reconnaissance de musiques dans une base de données
Le teensy écoute une musique et écrit dans le terminal de quel chanson/musique il s'agit, si elle est dans sa base de données
Sinon il dit qu'il ne la connait pas (et peut-être dit de laquelle elle est le plus proche)

#### Idée de comparaison de sons

Évolution de la fréquence en fonction du temps
Corrélation (auto/inter)
Empreinte audio (Acoustic fingerprint)
Cross-corrélation / FFT
