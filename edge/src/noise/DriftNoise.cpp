#include "noise/DriftNoise.hpp"
#include <algorithm>

DriftNoise::DriftNoise(double step_sigma, double max_drift)
    : step_sigma_(step_sigma),
      max_drift_(max_drift),
      drift_(0.0),
      gen_(std::random_device{}()),
      step_dist_(0.0, step_sigma_) {}

double DriftNoise::apply() {
    // Aggiorna il random walk
    drift_ += step_dist_(gen_);

    // Clamping fisico: la deriva non può superare max_drift in valore assoluto.
    // std::clamp richiede C++17.
    drift_ = std::clamp(drift_, -max_drift_, max_drift_);

    return drift_;
}