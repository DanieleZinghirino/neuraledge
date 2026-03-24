#pragma once
#include "IFaultModel.hpp"
#include <random>

/**
 * @brief Saturazione del sensore ai limiti del range di misura.
 *
 * MODELLO FISICO:
 * Ogni sensore ha un range fisico oltre il quale l'output si "incolla"
 * al valore massimo o minimo misurabile. Causato da:
 * - Grandezza fisica fuori range (es. temperatura > fondo scala)
 * - ADC che clippa l'ingresso analogico
 * - Alimentazione insufficiente al circuito di acquisizione
 *
 * MODELLO MATEMATICO:
 *   output = clamp(value, lower_bound, upper_bound)
 *
 * La saturazione è sempre attiva (non stocastica): modella un
 * sensore che opera strutturalmente fuori range.
 * Il guasto si attiva con una certa probabilità e, una volta attivo,
 * il clamp è permanente.
 *
 * RICONOSCIMENTO:
 * Lunghe sequenze di campioni identici al valore massimo o minimo
 * sono il segnale diagnostico classico.
 */
class SaturationFault : public IFaultModel {
    private:
        double lower_bound_;       // valore minimo restituibile
        double upper_bound_;       // valore massimo restituibile
        double activation_prob_;

        bool is_active_;

        std::mt19937 gen_;
        std::uniform_real_distribution<double> trigger_dist_;

    public:
        /**
        * @param lower_bound     Limite inferiore di saturazione.
        * @param upper_bound     Limite superiore di saturazione.
        * @param activation_prob Probabilità di attivazione per campione.
        */
        SaturationFault(double lower_bound, double upper_bound,
                        double activation_prob);

        double apply(double value) override;
};