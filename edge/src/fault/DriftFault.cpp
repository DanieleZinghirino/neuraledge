#include "fault/DriftFault.hpp"
#include <cmath>
#include <algorithm>

DriftFault::DriftFault(double activation_prob, double drift_rate,
                       double max_drift,        double exponent)
    : drift_rate_(drift_rate),
      exponent_(exponent),
      max_drift_(max_drift),
      step_(0.0),
      is_active_(false),
      direction_(1),
      gen_(std::random_device{}()),
      trigger_dist_(0.0, 1.0),
      dir_dist_(0, 1),
      activation_prob_(activation_prob) {}

double DriftFault::apply(double value) {
    // Prova ad attivare il guasto se non già attivo
    if (!is_active_) {
        if (trigger_dist_(gen_) < activation_prob_) {
            is_active_ = true;
            direction_ = (dir_dist_(gen_) == 0) ? 1 : -1;
        } else {
            return value;
        }
    }

    // Calcola la deriva al passo corrente: rate * step^exponent
    const double drift = drift_rate_ * std::pow(step_, exponent_);

    // Applica il limite fisico
    const double clamped = std::min(drift, max_drift_);

    step_ += 1.0;

    return value + direction_ * clamped;
}