#include "signal/CurrentSignal.hpp"
#include <cmath>

CurrentSignal::CurrentSignal(double base, double amplitude, double frequency)
    : base_(base), amplitude_(amplitude), frequency_(frequency) {}

double CurrentSignal::generate(double t) {
    return base_ + amplitude_ * std::sin(2.0 * M_PI * frequency_ * t);
}

std::string CurrentSignal::typeName() const { return "current"; }
std::string CurrentSignal::unit()     const { return "A";       }