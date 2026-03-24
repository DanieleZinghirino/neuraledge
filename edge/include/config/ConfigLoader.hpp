#pragma once

#include "AppConfig.hpp"
#include <string>

namespace config {

/**
 * @brief Loader della configurazione applicativa da file YAML.
 *
 * WHY:
 * - sposta la definizione dello scenario fuori dal codice;
 * - rende i run riproducibili;
 * - permette di versionare gli scenari nel repository.
 */
class ConfigLoader {
public:
    /**
     * @brief Carica e valida un file YAML.
     * @throws std::runtime_error in caso di file invalido o campi mancanti.
     */
    static AppConfig loadFromYaml(const std::string& path);
};

} // namespace config