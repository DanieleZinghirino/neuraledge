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
    // generate() è const ma dobbiamo modificare lo stato del transitorio.
    // Il cast è giustificato: lo stato del transitorio è implementation detail
    // del modello stocastico, non parte del valore logico dell'oggetto.
    auto* self = const_cast<PressureSignal*>(this);

    // 1. Baseline con oscillazione lenta
    double value = baseline_ +
        slow_amplitude_ * std::sin(2.0 * M_PI * slow_freq_ * t);

    // 2. Attivazione casuale di un transitorio (se non già in uno)
    if (!in_transient_ && trigger_dist_(self->gen_) < transient_prob_) {
        self->in_transient_     = true;
        self->transient_start_  = t;
        self->transient_sign_   = (sign_dist_(self->gen_) == 0) ? 1 : -1;
    }

    // 3. Applicazione del transitorio esponenziale corrente
    if (in_transient_) {
        const double t_rel     = t - transient_start_;
        const double transient = transient_mag_ *
            (1.0 - std::exp(-t_rel / transient_tau_));

        value += transient_sign_ * transient;

        // Il transitorio si considera esaurito al 99% della magnitudine
        if (transient >= transient_mag_ * 0.99) {
            self->in_transient_ = false;
        }
    }

    // 4. Clamping fisico — la pressione non può essere negativa né superare p_max
    return std::clamp(value, 0.0, p_max_);
}

std::string PressureSignal::typeName() const { return "pres"; }
std::string PressureSignal::unit()     const { return "bar";  }