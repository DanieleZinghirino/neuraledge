#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Segnale a onda quadra con duty cycle configurabile.
 *
 * MODELLO FISICO:
 * Modella segnali on/off di attuatori digitali: pompe, valvole,
 * riscaldatori, nastri trasportatori. Il segnale alterna tra
 * due livelli (high e low) con un periodo e un duty cycle configurabili.
 *
 * MODELLO MATEMATICO:
 *   phase = fmod(t, period) / period  → posizione nel ciclo [0, 1)
 *   S(t)  = high_level  se phase < duty_cycle
 *           low_level   altrimenti
 *
 * duty_cycle = 0.5 → onda simmetrica (50% on, 50% off)
 * duty_cycle = 0.3 → 30% on, 70% off (pompa a impulsi)
 * duty_cycle = 0.8 → 80% on, 20% off (riscaldatore quasi continuo)
 *
 * UNITÀ: configurabile (default "state" per segnali adimensionali)
 */
class SquareWaveSignal : public ISignalModel {
    private:
        double      period_;      // periodo completo in secondi
        double      duty_cycle_;  // frazione del periodo allo stato alto [0, 1]
        double      high_level_;  // valore allo stato alto
        double      low_level_;   // valore allo stato basso
        std::string unit_str_;    // unità di misura (configurabile per flessibilità)

    public:
        /**
        * @param period      Periodo del ciclo in secondi.
        * @param duty_cycle  Frazione del periodo allo stato alto [0.0, 1.0].
        * @param high_level  Valore allo stato alto (default 1.0).
        * @param low_level   Valore allo stato basso (default 0.0).
        * @param unit_str    Unità di misura (default "state").
        */
        SquareWaveSignal(double period,
                        double duty_cycle,
                        double high_level = 1.0,
                        double low_level  = 0.0,
                        std::string unit_str = "state");

        double      generate  (double t) const override;
        std::string typeName  ()         const override;
        std::string unit      ()         const override;
    };