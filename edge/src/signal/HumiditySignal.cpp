#include "signal/HumiditySignal.hpp"
#include <cmath>

HumiditySignal::HumiditySignal(double base, double amplitude,
                               double frequency, double k)
    : base_(base),
      amplitude_(amplitude),
      frequency_(frequency),
      k_(k) {}

double HumiditySignal::generate(double t) const {
    // Valore grezzo — può uscire dal range [0, 100]
    const double raw = base_ + amplitude_ * std::sin(2.0 * M_PI * frequency_ * t);

    // Proiezione sigmoidale → (0, 100)
    // La sigmoide comprime i valori estremi verso i boundary
    // in modo continuo e differenziabile, più realistico del clamp.
    return 100.0 / (1.0 + std::exp(-k_ * (raw - 50.0)));
}

std::string HumiditySignal::typeName() const { return "humi"; }
std::string HumiditySignal::unit()     const { return "%";    }