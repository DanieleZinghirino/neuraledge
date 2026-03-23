#pragma once

#include "ISensor.hpp"
#include "../signal/ISignalModel.hpp"
#include "../noise/INoiseModel.hpp"
#include "../fault/IFaultModel.hpp"

#include <memory>
#include <string>

/**
 * @brief Sensore composito (core del design)
 *
 * ARCHITETTURA:
 * Signal -> Noise -> Fault
 *
 * WHY:
 * - separazione delle responsabilità: ogni componente (Signal, Noise, Fault) è respons
 * - riusabilità: i componenti possono essere riutilizzati in contesti diversi
 * - estendibilità: è facile aggiungere nuovi tipi di Signal, Noise o Fault senza
 */

class Sensor : public ISensor {
    private:
        std::string id_;
        double sampling_rate_;
        double current_time_;

        std::unique_ptr<ISignalModel> signal_;
        std::unique_ptr<INoiseModel> noise_;
        std::unique_ptr<IFaultModel> fault_;
    public:
        Sensor(std::string id, double sampling_rate,
               std::unique_ptr<ISignalModel> signal,
               std::unique_ptr<INoiseModel> noise,
               std::unique_ptr<IFaultModel> fault);
        
        Sample read() override;

        double getSamplingRate() const { return sampling_rate_; }
};