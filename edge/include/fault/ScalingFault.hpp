#pragma once
#include "IFaultModel.hpp"
#include <random>

/**
 * @brief Errore di guadagno (gain error) — fattore di scala errato.
 *
 * MODELLO FISICO:
 * Il sensore misura correttamente le variazioni relative ma con
 * un fattore di scala sbagliato rispetto al valore assoluto.
 * Causato da deriva del circuito di amplificazione, perdita di
 * calibrazione, o sostituzione con un sensore di sensibilità diversa.
 *
 * ESEMPIO:
 * Un sensore di temperatura calibrato per [0°C, 100°C] viene usato
 * con un amplificatore tarato per [0°C, 120°C]: tutte le letture
 * risulteranno scalate del 20% rispetto al valore reale.
 *
 * MODELLO MATEMATICO:
 *   output = value * scale_factor
 *
 * scale_factor > 1.0 → letture sovrastimate
 * scale_factor < 1.0 → letture sottostimate
 * scale_factor = 1.0 → nessun errore (NoFault equivalente)
 *
 * Il guasto scatta con una certa probabilità e rimane permanente.
 */
class ScalingFault : public IFaultModel {
    private:
        double scale_factor_;      // fattore moltiplicativo applicato al valore
        double activation_prob_;   // probabilità di attivazione

        bool is_active_;

        std::mt19937 gen_;
        std::uniform_real_distribution<double> trigger_dist_;

    public:
        /**
        * @param scale_factor      Fattore di scala (es. 1.2 = +20%, 0.8 = -20%).
        * @param activation_prob   Probabilità di attivazione per campione.
        */
        ScalingFault(double scale_factor, double activation_prob);

        double apply(double value) override;
};