#include "signal/RampSignal.hpp"
#include <cmath>

RampSignal::RampSignal(double period, double low, double high)
    : period_(period), low_(low), high_(high) {}

double RampSignal::generate(double t) const {
    // Posizione normalizzata nel ciclo [0.0, 1.0)
    const double phase = std::fmod(t, period_) / period_;
    return low_ + (high_ - low_) * phase;
}

std::string RampSignal::typeName() const { return "ramp"; }
std::string RampSignal::unit()     const { return "%";    }