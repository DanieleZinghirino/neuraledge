#pragma once
#include "INoiseModel.hpp"
#include <random>

/****
 * @brief Modello di rumore gaussiano n(0, sigma).
 * - media 0, deviazione standard configurabile
 */
class GaussianNoise : public INoiseModel {
    private:
        std::mt19937 gen_;
        std::normal_distribution<> dist_;

    public:
        explicit GaussianNoise(double sigma);

        double apply() override;
};