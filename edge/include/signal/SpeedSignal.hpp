#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Modello velocità: moto uniformemente accelerato
 */
class SpeedSignal : public ISignalModel {
private:
    double v0_;
    double acceleration_;

public:
    SpeedSignal(double v0, double acceleration);

    double generate(double t) override;
};