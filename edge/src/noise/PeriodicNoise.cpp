#include "noise/PeriodicNoise.hpp"
#include <cmath>

PeriodicNoise::PeriodicNoise(double amplitude, double frequency,
                             double sampling_rate, double phase)
    : amplitude_(amplitude),
      frequency_(frequency),
      phase_(phase),
      sampling_rate_(sampling_rate),
      current_time_(0.0) {}

double PeriodicNoise::apply() {
    const double value = amplitude_ *
        std::sin(2.0 * M_PI * frequency_ * current_time_ + phase_);

    // Avanza il tempo di un periodo di campionamento
    current_time_ += 1.0 / sampling_rate_;

    return value;
}