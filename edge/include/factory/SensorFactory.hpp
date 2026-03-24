#pragma once

#include "config/SensorConfig.hpp"
#include "sensor/Sensor.hpp"

#include <random>
#include <vector>

/**
 * @brief Factory config-driven per la creazione dei sensori.
 *
 * WHY:
 * - il main non conosce i tipi concreti dei modelli;
 * - la composizione signal/noise/fault viene centralizzata;
 * - il file di configurazione diventa la fonte di verità dello scenario.
 */
class SensorFactory {
public:
    /**
     * @brief Crea un singolo sensore a partire da una SensorConfig.
     */
    static Sensor createFromConfig(const config::SensorConfig& cfg, std::mt19937& gen);

    /**
     * @brief Crea tutti i sensori dichiarati in configurazione.
     */
    static std::vector<Sensor> createAllFromConfig(
        const std::vector<config::SensorConfig>& configs,
        std::mt19937& gen
    );
};