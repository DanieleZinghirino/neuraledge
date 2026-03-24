#include "signal/CurrentSignal.hpp"
#include <cmath>

CurrentSignal::CurrentSignal(double base,       double fundamental, double freq,
                             double thd3_amp,   double thd3_phase,
                             double thd5_amp,   double thd5_phase,
                             double load_period,double delta_I,    double tau_load)
    : base_(base),
      fundamental_(fundamental),
      freq_(freq),
      thd3_amp_(thd3_amp),
      thd3_phase_(thd3_phase),
      thd5_amp_(thd5_amp),
      thd5_phase_(thd5_phase),
      load_period_(load_period),
      delta_I_(delta_I),
      tau_load_(tau_load) {}

double CurrentSignal::generate(double t) const {
    // 1. Fondamentale
    const double i1 = fundamental_ * std::sin(2.0 * M_PI * freq_ * t);

    // 2. Terza armonica — causa principale di distorsione in carichi trifase
    const double i3 = thd3_amp_ *
        std::sin(2.0 * M_PI * 3.0 * freq_ * t + thd3_phase_);

    // 3. Quinta armonica — tipica di raddrizzatori a 6 impulsi
    const double i5 = thd5_amp_ *
        std::sin(2.0 * M_PI * 5.0 * freq_ * t + thd5_phase_);

    // 4. Transitorio di carico periodico.
    //    t_rel è il tempo trascorso dall'ultimo transitorio.
    const double t_rel      = std::fmod(t, load_period_);
    const double i_transient = delta_I_ * (1.0 - std::exp(-t_rel / tau_load_));

    return base_ + i1 + i3 + i5 + i_transient;
}

std::string CurrentSignal::typeName() const { return "curr"; }
std::string CurrentSignal::unit()     const { return "A";    }