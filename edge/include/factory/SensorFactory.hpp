#pragma once

#include "sensor/Sensor.hpp"
#include <random>
#include <string>
#include <map>

/**
 * @brief Factory per la creazione di Sensor con tipo casuale.
 *
 * WHY FACTORY METHOD:
 * - Il main non conosce i tipi concreti di ISignalModel
 * - Aggiungere un nuovo tipo di segnale = modificare solo questa classe
 * - Principio Open/Closed: aperto all'estensione, chiuso alla modifica
 */
class SensorFactory {
public:
    /**
     * @brief Crea un Sensor con tipo di segnale scelto casualmente.
     *
     * @param gen    Generatore Mersenne Twister condiviso con il main
     * @return       Sensor completamente configurato con id incrementale
     */
    static Sensor createRandom(std::mt19937& gen);

private:
    /**
     * @brief Contatori per tipo — static per persistere tra le chiamate.
     * Ogni volta che viene creato un sensore di un certo tipo,
     * il contatore corrispondente viene incrementato.
     */
    static std::map<std::string, int> counters_;
};