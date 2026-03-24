#pragma once
#include "IFaultModel.hpp"
#include <random>

/**
 * @brief Deriva progressiva e irreversibile del sensore.
 *
 * MODELLO FISICO:
 * Degrado fisico del materiale sensibile: fouling (depositi
 * sul sensore), corrosione, o degrado del dielettrico.
 * A differenza del DriftNoise (random walk bidirezionale e limitato),
 * questo è un guasto monotonico: la deriva cresce sempre nella
 * stessa direzione e accelera nel tempo, simulando un deterioramento
 * che peggiora progressivamente.
 *
 * MODELLO MATEMATICO:
 *   drift(t) = direction * rate * t^exponent
 *
 * Con exponent > 1 la deriva accelera (degrado non lineare).
 * Con exponent = 1 la deriva è lineare (degrado costante).
 *
 * DIFFERENZA DA DriftNoise:
 * DriftNoise è rumore bidirezionale e limitato — parte del fondo.
 * DriftFault è monotonico, illimitato e si attiva con una certa probabilità.
 */
class DriftFault : public IFaultModel {
    private:
        double drift_rate_;    // velocità base della deriva per campione
        double exponent_;      // curvatura della deriva (1.0 = lineare, >1 = accelerante)
        double max_drift_;     // limite fisico assoluto (es. fondo scala del sensore)

        double step_;          // contatore dei campioni dall'attivazione
        bool   is_active_;     // il guasto è attivo?
        int    direction_;     // +1 o -1, scelto casualmente all'attivazione

        std::mt19937 gen_;
        std::uniform_real_distribution<double> trigger_dist_;
        std::uniform_int_distribution<int>     dir_dist_;

        double activation_prob_;  // probabilità di attivazione per campione

    public:
        /**
        * @param activation_prob  Probabilità di attivazione per campione.
        * @param drift_rate       Incremento base per campione dall'attivazione.
        * @param max_drift        Deriva massima assoluta applicabile.
        * @param exponent         Curvatura (default 1.5 — accelerante moderato).
        */
        DriftFault(double activation_prob, double drift_rate,
                double max_drift,       double exponent = 1.5);

        double apply(double value) override;
};