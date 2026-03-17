#pragma once
#include "IFaultModel.hpp"

/**
 * @brief Modello di guasto che non introduce alcuna anomalia.
 * - utile per test e come baseline
 */
class NoFault : public IFaultModel {
    public:
        double apply(double value) override;
};