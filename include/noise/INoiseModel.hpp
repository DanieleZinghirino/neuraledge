#pragma once

/**
 * @brief Interfaccia astratta per un modello di rumore.
 *
 * RESPONSABILITA:
 * - perturbazione del segnale
 *
 * ESEMPIO:
 * - rumore bianco
 * - rumore gaussiano
 *
 * NOTA:
 * - separato dal segnale -> composizione
 */
class INoiseModel {
    public:
        virtual ~INoiseModel() = default;

        /**
         * @brief applica il rumore e restituisce un valore da sommare al segnale ideale.
         */
        virtual double apply() = 0;
};
