#pragma once
#include "INoiseModel.hpp"
#include <random>

/**
 * @brief Rumore impulsivo (Salt & Pepper) — picchi brevi e intensi.
 *
 * MODELLO FISICO:
 * Disturbi elettromagnetici da commutazione di carichi, scariche
 * elettrostatiche o interferenze radio producono picchi istantanei
 * sul segnale acquisito. Si distingue da SpikeFault perché:
 * - è parte del rumore di fondo, non un guasto del sensore
 * - i picchi sono simmetrici (positivi e negativi con uguale probabilità)
 * - la probabilità è più alta e l'ampiezza tipicamente minore di uno spike fault
 *
 * MODELLO MATEMATICO:
 * Ad ogni campione, con probabilità p scatta un impulso la cui
 * ampiezza è estratta da N(0, impulse_sigma). La simmetria è
 * garantita dalla distribuzione normale centrata in zero.
 *
 * DIFFERENZA DA SpikeFault:
 * SpikeFault aggiunge sempre una magnitudine positiva fissa.
 * ImpulseNoise aggiunge un impulso casuale positivo o negativo.
 */
class ImpulseNoise : public INoiseModel {
    private:
        double probability_;    // probabilità di impulso per campione [0, 1]
        double impulse_sigma_;  // deviazione standard dell'ampiezza dell'impulso

        std::mt19937 gen_;
        std::uniform_real_distribution<double> trigger_dist_;  // per decidere se scatta
        std::normal_distribution<double>       impulse_dist_;  // per l'ampiezza

    public:
        /**
        * @param probability   Probabilità di impulso per campione (es. 0.02 = 2%).
        * @param impulse_sigma Deviazione standard dell'ampiezza dell'impulso.
        */
        ImpulseNoise(double probability, double impulse_sigma);

        double apply() override;
};