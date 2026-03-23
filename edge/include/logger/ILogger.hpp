#pragma once
#include <string>

/**
 * @brief Interfaccia astratta per il logging.
 *
 * WHY:
 * - Disaccoppia SensorRunner dal logger concreto (CsvLogger)
 * - Permette di sostituire il logger senza modificare il runner
 *   (es. JsonLogger, ConsoleLogger, NetworkLogger)
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    /**
     * @brief Inizializza il logger (es. crea il file, scrive l'header).
     */
    virtual void init() = 0;

    /**
     * @brief Scrive una riga di log.
     * @param line stringa da scrivere.
     */
    virtual void log(const std::string& line) = 0;
};
