#pragma once
#include "INoiseModel.hpp"
#include <random>

/**
 * @brief Rumore a distribuzione uniforme su [-amplitude, +amplitude].
 *
 * MODELLO FISICO:
 * Errore di quantizzazione ADC — quando un segnale analogico viene
 * digitalizzato, l'errore è distribuito uniformemente nel range del
 * bit meno significativo (LSB). Comune in tutti i sistemi con conversione A/D.
 *
 * DIFFERENZA DA GAUSSIANO:
 * Il rumore gaussiano ha code infinite (valori molto grandi sono rari
 * ma possibili). Il rumore uniforme è strettamente limitato: non supera
 * mai ±amplitude. Questo lo rende più adatto a modellare errori di
 * quantizzazione dove il bound è noto e fisso.
 */
class UniformNoise : public INoiseModel {
    private:
        std::mt19937 gen_;
        std::uniform_real_distribution<double> dist_;

    public:
        /**
        * @param amplitude  Semi-ampiezza dell'intervallo [-amplitude, +amplitude].
        *                   Corrisponde tipicamente a 0.5 * LSB del convertitore.
        */
        explicit UniformNoise(double amplitude);

        double apply() override;
};