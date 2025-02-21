#include "CompareMusic.h"
#include "StoreMelody.h"
#include <SD.h>
#include <vector>
#include <numeric>
#include <cmath>
#include <Arduino.h>

std::vector<float> readCSV(const char* filename) {
    std::vector<float> data;
    File file = SD.open(filename);
    if (!file) {
        Serial.print("Erreur d'ouverture du fichier : ");
        Serial.println(filename);
        return data;
    }
    while (file.available()) {
        data.push_back(file.parseFloat());
    }
    file.close();
    return data;
}

float calculateCorrelation(const std::vector<float>& a, const std::vector<float>& b) {
    int n = a.size();
    float mean_a = std::accumulate(a.begin(), a.end(), 0.0) / n;
    float mean_b = std::accumulate(b.begin(), b.end(), 0.0) / n;

    float sum_ab = 0.0;
    float sum_a2 = 0.0;
    float sum_b2 = 0.0;

    for (int i = 0; i < n; ++i) {
        sum_ab += (a[i] - mean_a) * (b[i] - mean_b);
        sum_a2 += (a[i] - mean_a) * (a[i] - mean_a);
        sum_b2 += (b[i] - mean_b) * (b[i] - mean_b);
    }

    return sum_ab / sqrt(sum_a2 * sum_b2);
}

void compareMusic(const char* targetFile) {
    std::vector<float> targetMelody = readCSV(targetFile);
    if (targetMelody.empty()) {
        Serial.println("Erreur de lecture de la mélodie cible.");
        return;
    }

    std::vector<String> storedMelodies = getStoredMelodies();
    float bestScore = -1.0;
    String bestMatch;

    for (String melodyFile : storedMelodies) {
        if (melodyFile == targetFile) continue; // Skip the target file itself

        std::vector<float> melody = readCSV(melodyFile.c_str());
        if (melody.empty()) continue;

        float score = calculateCorrelation(targetMelody, melody);
        if (score > bestScore) {
            bestScore = score;
            bestMatch = melodyFile;
        }
    }

    Serial.print("La chanson la plus similaire est : ");
    Serial.println(bestMatch);
    Serial.print("Score de corrélation : ");
    Serial.println(bestScore);
}