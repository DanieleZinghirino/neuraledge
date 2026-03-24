#include "fault/SaturationFault.hpp"
#include <algorithm>

SaturationFault::SaturationFault(double lower_bound, double upper_bound,
                                 double activation_prob)
    : lower_bound_(lower_bound),
      upper_bound_(upper_bound),
      activation_prob_(activation_prob),
      is_active_(false),
      gen_(std::random_device{}()),
      trigger_dist_(0.0, 1.0) {}

double SaturationFault::apply(double value) {
    if (!is_active_) {
        if (trigger_dist_(gen_) < activation_prob_) {
            is_active_ = true;
        } else {
            return value;
        }
    }

    return std::clamp(value, lower_bound_, upper_bound_);
}