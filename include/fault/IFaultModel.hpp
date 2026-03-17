#pragma once

/**
 * @brief Interfaccia astratta per un modello di anomalia/errore.
 *
 * DIFFERENZA CON NOISE:
 * - noise: piccolo e continuo
 * - fault: raro, strutturato, significativo
 */
class IFaultModel {
    public:
        virtual ~IFaultModel() = default;

        /**
         * @brief applica il guasto e restituisce un valore da sommare al segnale ideale.
         */
        virtual double apply(double value) = 0;
};
