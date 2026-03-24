#include "fault/ScalingFault.hpp"

ScalingFault::ScalingFault(double scale_factor, double activation_prob)
    : scale_factor_(scale_factor),
      activation_prob_(activation_prob),
      is_active_(false),
      gen_(std::random_device{}()),
      trigger_dist_(0.0, 1.0) {}

double ScalingFault::apply(double value) {
    if (!is_active_) {
        if (trigger_dist_(gen_) < activation_prob_) {
            is_active_ = true;
        } else {
            return value;
        }
    }

    return value * scale_factor_;
}