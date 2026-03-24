#include "noise/ImpulseNoise.hpp"

ImpulseNoise::ImpulseNoise(double probability, double impulse_sigma)
    : probability_(probability),
      impulse_sigma_(impulse_sigma),
      gen_(std::random_device{}()),
      trigger_dist_(0.0, 1.0),
      impulse_dist_(0.0, impulse_sigma_) {}

double ImpulseNoise::apply() {
    // Se non scatta, contribuisce zero — nessun rumore questo campione
    if (trigger_dist_(gen_) >= probability_) {
        return 0.0;
    }

    // Ampiezza casuale con segno — simmetrico positivo/negativo
    return impulse_dist_(gen_);
}