#include "signal/TemperatureSignal.hpp"
#include <cmath>

TemperatureSignal::TemperatureSignal(double base,
                                     double tau_warmup,
                                     double slow_amp,  double slow_freq,
                                     double fast_amp,  double fast_freq)
    : base_(base),
      tau_warmup_(tau_warmup),
      slow_amp_(slow_amp),
      slow_freq_(slow_freq),
      fast_amp_(fast_amp),
      fast_freq_(fast_freq) {}

double TemperatureSignal::generate(double t) const {
    const double warmup = (tau_warmup_ > 0.0)
        ? base_ * (1.0 - std::exp(-t / tau_warmup_))
        : base_;

    const double slow = slow_amp_ * std::sin(2.0 * M_PI * slow_freq_ * t);
    const double fast = fast_amp_ * std::sin(2.0 * M_PI * fast_freq_ * t);

    return warmup + slow + fast;
}

std::string TemperatureSignal::typeName() const { return "temp"; }
std::string TemperatureSignal::unit()     const { return "C";    }