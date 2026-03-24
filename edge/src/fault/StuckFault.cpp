#include "fault/StuckFault.hpp"

StuckFault::StuckFault(double stuck_prob)
    : stuck_prob_(stuck_prob),
      gen_(std::random_device{}()),
      trigger_dist_(0.0, 1.0),
      is_stuck_(false) {}

double StuckFault::apply(double value) {
    // Se già bloccato, restituisce sempre il valore congelato
    if (is_stuck_) {
        return stuck_value_.value();
    }

    // Prova ad attivare il guasto
    if (trigger_dist_(gen_) < stuck_prob_) {
        is_stuck_    = true;
        stuck_value_ = value;   // congela il valore corrente
        return stuck_value_.value();
    }

    return value;
}