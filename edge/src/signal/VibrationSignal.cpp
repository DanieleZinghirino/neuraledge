#include "signal/VibrationSignal.hpp"
#include <cmath>
#include <numeric>

VibrationSignal::VibrationSignal(double base_freq, std::vector<Harmonic> harmonics)
    : base_freq_(base_freq),
      harmonics_(std::move(harmonics)) {}

double VibrationSignal::generate(double t) const {
    // Somma di tutte le componenti armoniche.
    // std::accumulate su vector<Harmonic> — ogni elemento contribuisce
    // con la propria sinusoide alla frequenza i * f₀.
    return std::accumulate(
        harmonics_.begin(), harmonics_.end(), 0.0,
        [this, t](double sum, const Harmonic& h) {
            return sum + h.amplitude *
                std::sin(2.0 * M_PI * h.multiplier * base_freq_ * t + h.phase);
        }
    );
}

std::string VibrationSignal::typeName() const { return "vibr"; }
std::string VibrationSignal::unit()     const { return "mm/s"; }