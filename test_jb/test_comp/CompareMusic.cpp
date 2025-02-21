#include "CompareMusic.h"
#include "StoreMelody.h"
#include <numeric>
#include <cmath>
#include <Arduino.h>

bool compareMelodies(const std::vector<float>& capturedNotes, const std::vector<float>& storedMelody) {
    if (capturedNotes.size() != storedMelody.size()) {
        return false;
    }

    float correlation = calculateCorrelation(capturedNotes, storedMelody);
    return correlation > 0.8; // Seuil de corrélation pour considérer les mélodies comme correspondantes
}

String findMatchingMelody(const std::vector<float>& capturedNotes) {
    std::vector<String> storedMelodies = getStoredMelodies();
    float bestScore = -1.0;
    String bestMatch;

    for (String melodyFile : storedMelodies) {
        std::vector<float> storedMelody = loadMelody(melodyFile.c_str());
        if (storedMelody.empty()) continue;

        float score = calculateCorrelation(capturedNotes, storedMelody);
        if (score > bestScore) {
            bestScore = score;
            bestMatch = melodyFile;
        }
    }

    return bestMatch;
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