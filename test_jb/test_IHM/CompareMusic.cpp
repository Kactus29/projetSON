#include "include.h"

bool compareMelodies(const std::vector<float>& capturedNotes, const std::vector<float>& storedMelody) {
    if (capturedNotes.size() != storedMelody.size()) {
        return false;
    }

    float correlation = calculateCorrelation(capturedNotes, storedMelody);
    return correlation > 0.8; // Seuil de corrélation pour considérer les mélodies comme correspondantes
}

std::vector<std::pair<String, float>> findMatchingMelody(const std::vector<float>& capturedNotes) {
    std::vector<String> storedMelodies = getStoredMelodies(directory);
    std::vector<std::pair<String, float>> results;

    for (String melodyFile : storedMelodies) {
        std::vector<float> storedMelody = loadMelody(directory, melodyFile.c_str());
        if (storedMelody.empty() || storedMelody == capturedNotes) continue; // Skip if the melody is the same as captured

        float score = calculateCorrelation(capturedNotes, storedMelody);
        results.push_back(std::make_pair(melodyFile, score));
    }

    return results;
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
