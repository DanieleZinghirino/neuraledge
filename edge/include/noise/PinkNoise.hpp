#pragma once
#include "INoiseModel.hpp"
#include <random>
#include <array>

/**
 * @brief Rumore rosa (1/f noise) tramite approssimazione di Kellett.
 *
 * MODELLO FISICO:
 * La densità spettrale di potenza decresce proporzionalmente a 1/f.
 * È il rumore dominante nei sensori MEMS, sensori di pressione e
 * amplificatori operazionali a bassa frequenza. Suona più "naturale"
 * del rumore bianco perché le basse frequenze hanno più energia.
 *
 * ALGORITMO:
 * Approssimazione di Paul Kellett: somma di N generatori di rumore
 * bianco con stati persistenti, ognuno aggiornato con probabilità
 * decrescente (1/2, 1/4, 1/8...). Ogni livello contribuisce a una
 * diversa banda di frequenza. Il risultato normalizzato approssima
 * bene 1/f senza richiedere FFT.
 *
 * COMPLESSITÀ: O(N) per campione, N = numero di ottave (default 7).
 */
class PinkNoise : public INoiseModel {
    public:
        static constexpr int kOctaves = 7;

    private:
        double sigma_;
        std::mt19937 gen_;
        std::normal_distribution<double> white_;

        // Stato persistente dei generatori per ogni ottava
        std::array<double, kOctaves> rows_;
        double running_sum_;

        // Distribuzione per decidere quale ottava aggiornare
        std::uniform_int_distribution<int> octave_selector_;

    public:
        /**
        * @param sigma  Deviazione standard del rumore rosa prodotto.
        */
        explicit PinkNoise(double sigma);

        double apply() override;
};