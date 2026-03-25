#include "signal/PressureSignal.hpp"
#include <cmath>
#include <algorithm>

PressureSignal::PressureSignal(double baseline,
                               double slow_amplitude, double slow_freq,
                               double transient_prob, double transient_mag,
                               double transient_tau,  double p_max)
    : baseline_(baseline),
      slow_amplitude_(slow_amplitude),
      slow_freq_(slow_freq),
      transient_prob_(transient_prob),
      transient_mag_(transient_mag),
      transient_tau_(transient_tau),
      p_max_(p_max),
      in_transient_(false),
      transient_start_(0.0),
      transient_sign_(1),
      gen_(std::random_device{}()),
      trigger_dist_(0.0, 1.0),
      sign_dist_(0, 1) {}

double PressureSignal::generate(double t) const {
    // 1. Baseline con oscillazione lenta
    double value = baseline_ +
        slow_amplitude_ * std::sin(2.0 * M_PI * slow_freq_ * t);

    // 2. Attivazione casuale di un transitorio (se non già in uno).
    //    I membri sono mutable — modificabili anche in un metodo const.
    if (!in_transient_ && trigger_dist_(gen_) < transient_prob_) {
        in_transient_    = true;
        transient_start_ = t;
        transient_sign_  = (sign_dist_(gen_) == 0) ? 1 : -1;
    }

    // 3. Applicazione del transitorio esponenziale corrente
    if (in_transient_) {
        const double t_rel     = t - transient_start_;
        const double transient = transient_mag_ *
            (1.0 - std::exp(-t_rel / transient_tau_));

        value += transient_sign_ * transient;

        if (transient >= transient_mag_ * 0.99) {
            in_transient_ = false;
        }
    }

    // 4. Clamping fisico
    return std::clamp(value, 0.0, p_max_);
}

std::string PressureSignal::typeName() const { return "pres"; }
std::string PressureSignal::unit()     const { return "bar";  }