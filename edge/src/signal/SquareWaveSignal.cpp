#include "signal/SquareWaveSignal.hpp"
#include <cmath>
#include <utility>

SquareWaveSignal::SquareWaveSignal(double period, double duty_cycle,
                                   double high_level, double low_level,
                                   std::string unit_str)
    : period_(period),
      duty_cycle_(duty_cycle),
      high_level_(high_level),
      low_level_(low_level),
      unit_str_(std::move(unit_str)) {}

double SquareWaveSignal::generate(double t) const {
    // Posizione normalizzata nel ciclo corrente [0.0, 1.0)
    const double phase = std::fmod(t, period_) / period_;
    return (phase < duty_cycle_) ? high_level_ : low_level_;
}

std::string SquareWaveSignal::typeName() const { return "sq";       }
std::string SquareWaveSignal::unit()     const { return unit_str_;  }