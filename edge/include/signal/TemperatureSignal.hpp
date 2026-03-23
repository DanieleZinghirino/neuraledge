#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Modello di segnale per la temperatura.
 * - baseline + oscillazione lenta
 */
class TemperatureSignal : public ISignalModel {
    private:
        double base_; // temperatura media
        double amplitude_; // ampiezza dell'oscillazione
        double frequency_; // frequenza dell'oscillazione
    public:
        TemperatureSignal(double base, double amplitude, double frequency);
        
        double generate(double t) override;

        std::string typeName() const override;
        std::string unit()     const override;
};
