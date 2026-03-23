#include "sensor/Sensor.hpp"
#include "factory/SensorFactory.hpp"
#include "logger/CsvLogger.hpp"
#include "runner/SensorRunner.hpp"

#include <iostream>
#include <vector>
#include <random>

int main() {
    int num_sensors;
    std::cout << "Insert number of sensors: ";
    std::cin >> num_sensors;

    std::vector<Sensor> sensors;

    // Il gen viene passato alla factory così la sequenza random
    // è controllata da un unico punto (riproducibile con seed fisso)
    std::random_device rd;
    std::mt19937 gen(rd());

    // 🔴 Logger
    CsvLogger logger("../../output/sensor_data.csv");
    logger.init();

    // 🟢 Sensori — il main non sa nulla dei tipi concreti
    for (int i = 0; i < num_sensors; ++i) {
        sensors.push_back(SensorFactory::createRandom(gen));
    }

    // 🔵 Runner
    SensorRunner runner(sensors, logger);
    runner.run();

    std::cout << "Simulation completed\n";
    return 0;
}