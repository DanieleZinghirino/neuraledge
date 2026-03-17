#pragma once
#include "IFaultModel.hpp"
#include <random>

/**
 * @brief Modello di guasto a picco (spike fault) casuali.
 */
class SpikeFault : public IFaultModel {
    private:
        double magnitude_; // ampiezza del picco
        double probability_;  // probabilità di attivazione del picco

        std::mt19937 gen_;
        std::uniform_real_distribution<> dist_; // distribuzione uniforme [0, 1]
    public:
        SpikeFault(double prob, double mag);

        double apply(double value) override;
};
