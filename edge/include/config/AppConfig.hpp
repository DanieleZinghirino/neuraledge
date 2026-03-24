#pragma once

#include "SensorConfig.hpp"
#include <string>
#include <vector>

namespace config {

/**
 * @brief Parametri globali della simulazione.
 */
struct SimulationConfig {
    unsigned int seed {42};
    double duration_seconds {10.0};
    std::string output_csv {"../../../output/sensor_data.csv"};
};

/**
 * @brief Configurazione completa dell'applicazione.
 */
struct AppConfig {
    SimulationConfig simulation;
    std::vector<SensorConfig> sensors;
};

} // namespace config