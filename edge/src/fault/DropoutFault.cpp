#include "fault/DropoutFault.hpp"
#include <cmath>

DropoutFault::DropoutFault(double dropout_prob, Mode mode, int burst_length)
    : dropout_prob_(dropout_prob),
      burst_length_(burst_length),
      mode_(mode),
      remaining_dropout_(0),
      in_dropout_(false),
      gen_(std::random_device{}()),
      trigger_dist_(0.0, 1.0) {}

double DropoutFault::apply(double value) {
    if (mode_ == Mode::RANDOM) {
        // Ogni campione è indipendente
        if (trigger_dist_(gen_) < dropout_prob_) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return value;
    }

    // Modalità BURST
    if (in_dropout_) {
        --remaining_dropout_;
        if (remaining_dropout_ <= 0) {
            in_dropout_ = false;
        }
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Prova ad attivare un nuovo burst
    if (trigger_dist_(gen_) < dropout_prob_) {
        in_dropout_          = true;
        remaining_dropout_   = burst_length_ - 1;
        return std::numeric_limits<double>::quiet_NaN();
    }

    return value;
}