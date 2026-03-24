#include "runner/SensorRunner.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

SensorRunner::SensorRunner(std::vector<Sensor>& sensors,
                           ILogger& logger,
                           double duration_seconds)
    : sensors_(sensors),
      logger_(logger),
      duration_seconds_(duration_seconds) {}

void SensorRunner::worker(Sensor& sensor) {
    const int sleep_ms = static_cast<int>(std::round(1000.0 / sensor.getSamplingRate()));

    const std::size_t total_samples =
        std::max<std::size_t>(
            1,
            static_cast<std::size_t>(std::ceil(duration_seconds_ * sensor.getSamplingRate()))
        );

    for (std::size_t i = 0; i < total_samples; ++i) {
        auto s = sensor.read();

        {
            std::lock_guard<std::mutex> lock(cout_mtx_);

            if (std::isnan(s.value)) {
                std::cout << s.timestamp << " | "
                          << s.sensor_id << " | "
                          << "[DROPOUT]"
                          << std::endl;
            } else {
                std::cout << s.timestamp << " | "
                          << s.sensor_id << " | "
                          << s.value << " "
                          << s.unit
                          << std::endl;
            }
        }

        const std::string value_str = std::isnan(s.value) ? "" : std::to_string(s.value);

        const std::string line =
            s.timestamp + "," +
            s.sensor_id + "," +
            value_str + "," +
            s.unit;

        logger_.log(line);

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    }
}

void SensorRunner::run() {
    threads_.clear();
    threads_.reserve(sensors_.size());

    for (auto& sensor : sensors_) {
        threads_.emplace_back(&SensorRunner::worker, this, std::ref(sensor));
    }

    for (auto& t : threads_) {
        t.join();
    }
}