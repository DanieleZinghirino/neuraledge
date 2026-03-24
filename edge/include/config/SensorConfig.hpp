#pragma once

#include "ModelConfig.hpp"
#include <string>

namespace config {

/**
 * @brief Configurazione dichiarativa di un singolo sensore.
 *
 * type:
 * - categoria semantica del sensore (temperature, vibration, ...)
 * - utile per leggibilità, validazione e metadati
 *
 * signal / noise / fault:
 * - descrivono la pipeline del Sensor.
 */
    struct SensorConfig {
        std::string id;
        std::string type;
        double sampling_rate{1.0};

        ModelConfig signal;
        ModelConfig noise;
        ModelConfig fault;
    };
}