#pragma once
#include <string>

/**
 * @brief Interfaccia astratta per un modello di segnale senza rumore (ideale).
 *
 * RESPONSABILITA:
 * - modellare un fenomeno fisico
 *
 * ESEMPIO: 
 * - temperatura
 * - vibrazione
 */
class ISignalModel {
    public:
        virtual ~ISignalModel() = default;

        /**
         * @brief Genera un valore del segnale in un dato istante.
         *
         * @param t tempo generato.
         * @return valore ideale del segnale (senza rumore).
         */
        virtual double generate(double t) const = 0;

        virtual std::string typeName() const = 0;

        virtual std::string unit() const = 0;
};
