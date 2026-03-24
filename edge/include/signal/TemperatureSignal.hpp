#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Segnale di temperatura con modello termico realistico.
 *
 * MODELLO FISICO:
 * La temperatura in un ambiente industriale è influenzata da
 * tre componenti sovrapposte:
 *
 *   1. Ciclo lento (termico ambientale):
 *      oscillazione sinusoidale lenta che modella il ciclo
 *      termico del processo produttivo o della giornata.
 *
 *   2. Risposta termica all'avvio (warm-up):
 *      all'accensione il sistema parte freddo e converge
 *      esponenzialmente alla temperatura di regime.
 *      T_warmup(t) = base * (1 - e^(-t / tau_warmup))
 *
 *   3. Perturbazione ad alta frequenza:
 *      piccola oscillazione rapida causata da cicli
 *      di controllo del termoregolatore (on/off del riscaldatore).
 *
 * MODELLO MATEMATICO COMPLETO:
 *   T(t) = T_warmup(t) + A_slow * sin(2π * f_slow * t)
 *                      + A_fast * sin(2π * f_fast * t)
 *
 * UNITÀ: °C
 */
class TemperatureSignal : public ISignalModel {
private:
    double base_;          // temperatura di regime in °C
    double tau_warmup_;    // costante di tempo del warm-up in secondi
    double slow_amp_;      // ampiezza del ciclo termico lento
    double slow_freq_;     // frequenza del ciclo lento in Hz
    double fast_amp_;      // ampiezza della perturbazione del termoregolatore
    double fast_freq_;     // frequenza della perturbazione in Hz

public:
    /**
     * @param base         Temperatura di regime (°C).
     * @param tau_warmup   Costante di tempo del warm-up (s). 0 = no warm-up.
     * @param slow_amp     Ampiezza oscillazione lenta (°C).
     * @param slow_freq    Frequenza oscillazione lenta (Hz).
     * @param fast_amp     Ampiezza perturbazione termoregolatore (°C).
     * @param fast_freq    Frequenza perturbazione (Hz).
     */
    TemperatureSignal(double base,
                      double tau_warmup,
                      double slow_amp,  double slow_freq,
                      double fast_amp,  double fast_freq);

    double      generate  (double t) const override;
    std::string typeName  ()         const override;
    std::string unit      ()         const override;
};