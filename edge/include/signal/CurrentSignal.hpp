#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Segnale di corrente con forma d'onda realistica e transitori di carico.
 *
 * MODELLO FISICO:
 * La corrente assorbita da un carico industriale non è puramente
 * sinusoidale: contiene armoniche (distorsione armonica) causate
 * da carichi non lineari (inverter, raddrizzatori, alimentatori switching)
 * e subisce variazioni brusche durante transitori di carico
 * (avvio motore, inserzione di un carico pesante).
 *
 * MODELLO MATEMATICO:
 *   I(t) = I_dc
 *        + A₁ * sin(2π * f₁ * t)          → fondamentale (50/60 Hz)
 *        + A₃ * sin(2π * 3*f₁ * t + φ₃)   → 3ª armonica (THD)
 *        + A₅ * sin(2π * 5*f₁ * t + φ₅)   → 5ª armonica (THD)
 *        + I_transient(t)                   → transitorio di carico
 *
 * Il transitorio di carico è un gradino esponenziale:
 *   I_trans(t_rel) = delta_I * (1 - e^(-t_rel / tau_load))
 *
 * attivato periodicamente ogni load_period secondi.
 *
 * UNITÀ: A (Ampere)
 */
class CurrentSignal : public ISignalModel {
private:
    double base_;           // componente DC / corrente media
    double fundamental_;    // ampiezza della fondamentale (A)
    double freq_;           // frequenza fondamentale in Hz (50 o 60)

    // Armoniche di distorsione (THD)
    double thd3_amp_;       // ampiezza 3ª armonica
    double thd3_phase_;     // fase 3ª armonica (rad)
    double thd5_amp_;       // ampiezza 5ª armonica
    double thd5_phase_;     // fase 5ª armonica (rad)

    // Transitorio di carico
    double load_period_;    // periodo tra transitori di carico (s)
    double delta_I_;        // ampiezza del gradino di corrente (A)
    double tau_load_;       // costante di tempo del transitorio (s)

public:
    /**
     * @param base          Corrente media / componente DC (A).
     * @param fundamental   Ampiezza della fondamentale (A).
     * @param freq          Frequenza fondamentale (Hz) — tipicamente 50.0.
     * @param thd3_amp      Ampiezza 3ª armonica (A) — tipicamente 10-15% della fondamentale.
     * @param thd3_phase    Fase 3ª armonica (rad).
     * @param thd5_amp      Ampiezza 5ª armonica (A) — tipicamente 5-8%.
     * @param thd5_phase    Fase 5ª armonica (rad).
     * @param load_period   Periodo tra transitori di carico (s).
     * @param delta_I       Ampiezza del gradino di carico (A).
     * @param tau_load      Costante di tempo del transitorio di carico (s).
     */
    CurrentSignal(double base,       double fundamental, double freq,
                  double thd3_amp,   double thd3_phase,
                  double thd5_amp,   double thd5_phase,
                  double load_period,double delta_I,    double tau_load);

    double      generate  (double t) const override;
    std::string typeName  ()         const override;
    std::string unit      ()         const override;
};