#include "runner/SensorRunner.hpp"
#include <chrono>
#include <iostream>

SensorRunner::SensorRunner(std::vector<Sensor>& sensors, ILogger& logger)
    : sensors_(sensors), logger_(logger) {}

void SensorRunner::worker(Sensor& sensor) {
    const int sleep_ms = static_cast<int>(1000.0 / sensor.getSamplingRate());

    for (int i = 0; i < 100; ++i) {
        auto s = sensor.read();

        {
            std::lock_guard<std::mutex> lock(cout_mtx_);
            std::cout << s.timestamp << " | "
                      << s.sensor_id << " | "
                      << s.value
                      << s.unit << std::endl;
        }

        std::string line =
            s.timestamp + "," + s.sensor_id + "," + std::to_string(s.value) + "," + s.unit;

        logger_.log(line);

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
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