#include "runner/SensorRunner.hpp"
#include <chrono>
#include <random>
#include <iostream>

static std::random_device rd;
static std::mt19937 gen(rd());

SensorRunner::SensorRunner(std::vector<Sensor>& sensors, CsvLogger& logger)
    : sensors_(sensors), logger_(logger) {}

void SensorRunner::worker(Sensor& sensor) {
    std::uniform_real_distribution<double> sleep_dist(500, 2000);

    for (int i = 0; i < 100; ++i) {
        auto s = sensor.read();

        std::this_thread::sleep_for(
            std::chrono::milliseconds((int)sleep_dist(gen))
        );

        std::cout << s.timestamp << " | "
                  << s.sensor_id << " | "
                  << s.value << std::endl;

        std::string line =
            s.timestamp + "," + s.sensor_id + "," + std::to_string(s.value);

        logger_.log(line);
    }
}

void SensorRunner::run() {
    for (auto& sensor : sensors_) {
        threads_.emplace_back(&SensorRunner::worker, this, std::ref(sensor));
    }

    for (auto& t : threads_) {
        t.join();
    }
}