#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Modello di segnale per la corrente.
 */
class CurrentSignal : public ISignalModel {
private:
    double base_;
    double amplitude_;
    double frequency_;

public:
    CurrentSignal(double base, double amplitude, double frequency);

    double generate(double t) override;
};

