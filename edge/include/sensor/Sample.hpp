#pragma once
#include <string>

/**
 * @brief Rappresenta una singola misura prodotta da un sensore.
 *
 * DESIGN CHOICE:
 * - Struct POD-like: semplice, serializzabile, efficiente.
 * - timestamp double: flessibile (secondi, simulazione time, ecc.)
 */
 
struct Sample {
    std::string sensor_id;
    double value;
    std::string timestamp;
};