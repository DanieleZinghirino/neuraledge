#include "signal/StepResponseSignal.hpp"
#include <cmath>

StepResponseSignal::StepResponseSignal(double high_target, double low_target,
                                       double tau,          double step_period)
    : high_target_(high_target),
      low_target_(low_target),
      tau_(tau),
      step_period_(step_period) {}

double StepResponseSignal::generate(double t) const {
    // Identifica la fase corrente e il tempo trascorso dall'ultimo gradino.
    // Le fasi si alternano con periodo step_period_:
    //   fase pari  → target = high_target_
    //   fase dispari → target = low_target_
    const double full_cycle = 2.0 * step_period_;
    const double t_in_cycle = std::fmod(t, full_cycle);

    double y_target;
    double y_start;
    double t_rel;

    if (t_in_cycle < step_period_) {
        // Fase di riscaldamento → verso high_target
        y_target = high_target_;
        y_start  = low_target_;
        t_rel    = t_in_cycle;
    } else {
        // Fase di raffreddamento → verso low_target
        y_target = low_target_;
        y_start  = high_target_;
        t_rel    = t_in_cycle - step_period_;
    }

    // Risposta esponenziale al gradino
    return y_start + (y_target - y_start) * (1.0 - std::exp(-t_rel / tau_));
}

std::string StepResponseSignal::typeName() const { return "step"; }
std::string StepResponseSignal::unit()     const { return "C";    }