#pragma once
#include "Sample.hpp"

/**
 * @brief Interfaccia astratta per qualsiasi sensore.
 *
 * WHY:
 * - Polimorfismo runtime: consente di trattare diversi tipi di sensori in modo uniforme.
 * - Possibilità di utilizzare container di sensori eterogenei (es. std::vector<std::unique_ptr<ISensor>>).
 * ALTERNATIVE:
 * - Template (compile-time polymorphism): più efficiente, ma meno flessibile (non consente container eterogenei).
 */

class ISensor {
    public:
        virtual ~ISensor() = default;

        /**
         * @brief Legge i dati dal sensore.
         */
        virtual Sample read() = 0;
};
