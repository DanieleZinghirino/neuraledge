#include "sensor/Sensor.hpp"
#include "signal/TemperatureSignal.hpp"
#include "signal/CurrentSignal.hpp"
#include "signal/SpeedSignal.hpp"
#include "noise/GaussianNoise.hpp"
#include "fault/SpikeFault.hpp"

#include "logger/CsvLogger.hpp"
#include "runner/SensorRunner.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <filesystem>

int main() {
    int num_sensors;
    std::cout << "Insert number of sensors: ";
    std::cin >> num_sensors;

    std::vector<Sensor> sensors;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dis_rate(1.0, 3.0);
    std::uniform_real_distribution<double> dis_temp(15.0, 35.0);
    std::uniform_real_distribution<double> dis_amp(0.5, 3.0);
    std::uniform_real_distribution<double> dis_freq(0.01, 0.1);
    std::uniform_real_distribution<double> dis_noise(0.1, 0.6);
    std::uniform_real_distribution<double> dis_prob(0.0, 0.6);
    std::uniform_real_distribution<double> dis_spike(0.0, 15.0);

    // 🔴 Logger
    CsvLogger logger("../../output/sensor_data.csv");
    logger.init();

    // 🟢 Sensori
    for (int i = 0; i < num_sensors; ++i) {
        sensors.emplace_back(
            "curr" + std::to_string(i + 1),
            dis_rate(gen),
            std::make_unique<CurrentSignal>(
                dis_temp(gen), dis_amp(gen), dis_freq(gen)),
            std::make_unique<GaussianNoise>(dis_noise(gen)),
            std::make_unique<SpikeFault>(dis_prob(gen), dis_spike(gen))
        );
    }

    // 🔵 Runner
    SensorRunner runner(sensors, logger);
    runner.run();

    std::cout << "Simulation completed\n";
    return 0;
}