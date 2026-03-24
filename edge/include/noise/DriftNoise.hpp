#pragma once
#include "INoiseModel.hpp"
#include <random>

/**
 * @brief Rumore di deriva (random walk) — errore sistematico lentamente variabile.
 *
 * MODELLO FISICO:
 * I sensori fisici sono soggetti a deriva termica e invecchiamento:
 * il loro offset cambia lentamente nel tempo in modo non deterministico.
 * Modellato come random walk: ogni campione aggiunge un piccolo
 * incremento casuale alla deriva accumulata.
 *
 *   drift(t) = drift(t-1) + N(0, step_sigma)
 *
 * La deriva è illimitata nel tempo (assenza di mean reversion),
 * replicando il comportamento reale di sensori non ricalibrati.
 *
 * DIFFERENZA DA DriftFault:
 * Questo è rumore di fondo lento e continuo (sempre attivo).
 * DriftFault è un guasto progressivo e irreversibile con velocità crescente.
 *
 * PARAMETRI TIPICI:
 * step_sigma piccolo (0.001 - 0.01) per derive realistiche.
 * max_drift limita la deriva totale per evitare valori non fisici.
 */
class DriftNoise : public INoiseModel {
    private:
        double       step_sigma_;   // sigma dell'incremento per campione
        double       max_drift_;    // limite assoluto della deriva accumulata
        double       drift_;        // stato interno: deriva corrente

        std::mt19937 gen_;
        std::normal_distribution<double> step_dist_;

    public:
        /**
        * @param step_sigma  Deviazione standard dell'incremento per campione.
        * @param max_drift   Valore massimo assoluto della deriva (clamping).
        */
        DriftNoise(double step_sigma, double max_drift);

        double apply() override;
};