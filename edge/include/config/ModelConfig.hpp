#pragma once

#include <string>
#include <unordered_map>

namespace config {

/**
 * @brief Configurazione generica di un modello (signal, noise, fault).
 *
 * DESIGN:
 * - model: nome del modello concreto da istanziare
 * - numeric_params: parametri numerici (double)
 * - string_params: parametri testuali
 *
 * Perché separare parametri numerici e stringa:
 * - molti modelli usano solo double;
 * - alcuni richiedono enum/nomi simbolici (es. dropout mode = burst/random).
 */
struct ModelConfig {
    std::string model;
    std::unordered_map<std::string, double> numeric_params;
    std::unordered_map<std::string, std::string> string_params;
};

} // namespace config