#include "signal/SpeedSignal.hpp"
#include <cmath>
#include <algorithm>

SpeedSignal::SpeedSignal(double v_max,
                         double t_acc, double t_cruise,
                         double t_dec, double t_stop)
    : v_max_(v_max),
      t_acc_(t_acc),
      t_cruise_(t_cruise),
      t_dec_(t_dec),
      cycle_period_(t_acc + t_cruise + t_dec + t_stop) {}

double SpeedSignal::generate(double t) const {
    // Posizione nel ciclo corrente
    const double t_rel = std::fmod(t, cycle_period_);

    if (t_rel < t_acc_) {
        // Fase 1 — Accelerazione lineare da 0 a v_max
        return v_max_ * (t_rel / t_acc_);
    }

    const double t_after_acc = t_rel - t_acc_;

    if (t_after_acc < t_cruise_) {
        // Fase 2 — Crociera a velocità costante
        return v_max_;
    }

    const double t_after_cruise = t_after_acc - t_cruise_;

    if (t_after_cruise < t_dec_) {
        // Fase 3 — Decelerazione lineare da v_max a 0
        return v_max_ * (1.0 - t_after_cruise / t_dec_);
    }

    // Fase 4 — Fermo
    return 0.0;
}

std::string SpeedSignal::typeName() const { return "speed"; }
std::string SpeedSignal::unit()     const { return "m/s";   }