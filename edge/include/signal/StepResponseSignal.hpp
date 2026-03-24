#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Risposta esponenziale a gradini periodici.
 *
 * MODELLO FISICO:
 * Qualsiasi sistema fisico con inerzia termica, idraulica o meccanica
 * risponde a un ingresso a gradino con una curva esponenziale.
 * Esempi reali:
 * - Sensore di temperatura che misura il riscaldamento di un componente
 * - Livello di un serbatoio riempito con portata costante con perdite
 * - Velocità di un motore che accelera verso il setpoint
 *
 * MODELLO MATEMATICO:
 * Il segnale alterna tra due gradini con periodo step_period.
 * In ciascuna fase, la risposta segue:
 *
 *   y(t_rel) = y_start + (y_target - y_start) * (1 - e^(-t_rel / tau))
 *
 * dove:
 *   y_start  → valore all'inizio della fase corrente
 *   y_target → setpoint della fase corrente (high o low)
 *   tau      → costante di tempo del sistema (secondi)
 *   t_rel    → tempo trascorso dall'ultimo gradino
 *
 * UNITÀ: °C (modella tipicamente temperatura, ma generalizzabile)
 */
class StepResponseSignal : public ISignalModel {
    private:
        double high_target_;   // setpoint della fase alta
        double low_target_;    // setpoint della fase bassa
        double tau_;           // costante di tempo del sistema (secondi)
        double step_period_;   // durata di ciascuna fase (secondi)

    public:
        /**
        * @param high_target   Setpoint della fase alta (es. temperatura max).
        * @param low_target    Setpoint della fase bassa (es. temperatura min).
        * @param tau           Costante di tempo del sistema in secondi.
        * @param step_period   Durata di ciascuna fase in secondi.
        */
        StepResponseSignal(double high_target, double low_target,
                        double tau,         double step_period);

        double      generate  (double t) const override;
        std::string typeName  ()         const override;
        std::string unit      ()         const override;
};