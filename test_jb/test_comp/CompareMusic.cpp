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

std::vector<std::pair<String, float>> findMatchingMelody(const std::vector<float>& capturedNotes) {
    std::vector<String> storedMelodies = getStoredMelodies();
    std::vector<std::pair<String, float>> results;

    for (String melodyFile : storedMelodies) {
        std::vector<float> storedMelody = loadMelody(melodyFile.c_str());
        if (storedMelody.empty()) continue;

        float score = calculateDTW(capturedNotes, storedMelody);
        results.push_back(std::make_pair(melodyFile, score));
    }

    return results;
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

float calculateDTW(const std::vector<float>& a, const std::vector<float>& b) {
    int n = a.size();
    int m = b.size();
    std::vector<std::vector<float>> dtw(n + 1, std::vector<float>(m + 1, std::numeric_limits<float>::max()));
    dtw[0][0] = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            float cost = std::abs(a[i - 1] - b[j - 1]);
            dtw[i][j] = cost + std::min({dtw[i - 1][j], dtw[i][j - 1], dtw[i - 1][j - 1]});
        }
    }

    return dtw[n][m];
}
