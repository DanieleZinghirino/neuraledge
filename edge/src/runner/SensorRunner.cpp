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

        // =========================================================
        // THROTTLE CONSOLE — stampa al massimo kMaxConsolePrintHz
        // volte al secondo per sensore, indipendentemente dal
        // sampling rate. I dati vengono sempre scritti nel CSV.
        // =========================================================
        const auto now = std::chrono::steady_clock::now();
        bool should_print = false;

        {
            std::lock_guard<std::mutex> lock(last_print_mtx_);
            auto& last = last_print_[s.sensor_id];
            const double elapsed_ms = std::chrono::duration<double, std::milli>(now - last).count();

            if (elapsed_ms >= (1000.0 / kMaxConsolePrintHz)) {
                last         = now;
                should_print = true;
            }
        }

        if (should_print) {
            std::lock_guard<std::mutex> lock(cout_mtx_);

            if (std::isnan(s.value)) {
                std::cout << s.timestamp << " | "
                          << s.sensor_id << " | "
                          << "[DROPOUT]"
                          << "\n";
            } else {
                std::cout << s.timestamp << " | "
                          << s.sensor_id << " | "
                          << s.value     << " "
                          << s.unit
                          << "\n";
            }
        }

        // Log CSV — SEMPRE, nessun throttle
        const std::string value_str = std::isnan(s.value) ? "" : std::to_string(s.value);
        const std::string line =
            s.timestamp + "," +
            s.sensor_id + "," +
            value_str   + "," +
            s.unit;

        logger_.log(line);

        if (sleep_ms > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
        }
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