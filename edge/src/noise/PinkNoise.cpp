#include "noise/PinkNoise.hpp"
#include <numeric>

PinkNoise::PinkNoise(double sigma)
    : sigma_(sigma),
      gen_(std::random_device{}()),
      white_(0.0, 1.0),
      rows_{},
      running_sum_(0.0),
      octave_selector_(0, kOctaves - 1)
{
    // Inizializza ogni ottava con un campione bianco
    for (auto& r : rows_) {
        r = white_(gen_);
    }
    running_sum_ = std::accumulate(rows_.begin(), rows_.end(), 0.0);
}

double PinkNoise::apply() {
    // Seleziona casualmente una ottava da aggiornare
    const int idx = octave_selector_(gen_);

    const double prev = rows_[idx];
    rows_[idx] = white_(gen_);

    // Aggiorna la somma in modo incrementale — evita di ricalcolare tutto
    running_sum_ += rows_[idx] - prev;

    // Normalizza: la somma di kOctaves variabili gaussiane ha
    // deviazione standard sqrt(kOctaves), quindi dividiamo per essa
    return (running_sum_ / std::sqrt(kOctaves)) * sigma_;
}