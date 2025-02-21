#include "CompareMusic.h"
#include "StoreMelody.h"
#include <numeric>
#include <cmath>
#include <Arduino.h>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

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
    int m = b.size();
    
    if (n == 0 || m == 0) return 0.0; // Éviter les erreurs sur des vecteurs vides

    // Normalisation des vecteurs
    auto normalize = [](std::vector<float> v) {
        float mean = std::accumulate(v.begin(), v.end(), 0.0) / v.size();
        float max_val = *std::max_element(v.begin(), v.end());
        float min_val = *std::min_element(v.begin(), v.end());
        float range = (max_val - min_val == 0) ? 1.0 : (max_val - min_val);
        for (float& x : v) x = (x - mean) / range;
        return v;
    };

    std::vector<float> norm_a = normalize(a);
    std::vector<float> norm_b = normalize(b);

    float best_correlation = -1.0;

    // Tester tous les décalages possibles
    for (int shift = 0; shift <= std::abs(n - m); ++shift) {
        float sum_ab = 0.0, sum_a2 = 0.0, sum_b2 = 0.0;
        int min_size = std::min(n, m);

        for (int i = 0; i < min_size; ++i) {
            float val_a = (n > m) ? norm_a[i + shift] : norm_a[i];
            float val_b = (m > n) ? norm_b[i + shift] : norm_b[i];

            sum_ab += val_a * val_b;
            sum_a2 += val_a * val_a;
            sum_b2 += val_b * val_b;
        }

        float correlation = (sum_a2 > 0 && sum_b2 > 0) ? sum_ab / sqrt(sum_a2 * sum_b2) : 0.0;
        best_correlation = std::max(best_correlation, correlation);
    }

    return best_correlation;
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