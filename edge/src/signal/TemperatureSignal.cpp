#include "signal/TemperatureSignal.hpp"
#include <cmath>

TemperatureSignal::TemperatureSignal(double base, double amp, double freq)
    : base_(base), amplitude_(amp), frequency_(freq) {}

double TemperatureSignal::generate(double t) {
    // Modello: T(t) = base + amplitude * sin(2 * pi * frequency * t)
    return base_ + amplitude_ * std::sin(2 * M_PI * frequency_ * t);
}