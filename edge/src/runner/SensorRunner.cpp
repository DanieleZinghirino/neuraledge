#include "runner/SensorRunner.hpp"
#include <chrono>
#include <cmath>
#include <iostream>

SensorRunner::SensorRunner(std::vector<Sensor>& sensors, ILogger& logger)
    : sensors_(sensors), logger_(logger) {}

void SensorRunner::worker(Sensor& sensor) {
    const int sleep_ms = static_cast<int>(1000.0 / sensor.getSamplingRate());

    for (int i = 0; i < 100; ++i) {
        auto s = sensor.read();

        // =========================================================
        // STAMPA — protetta da mutex per evitare interleaving
        // tra thread diversi su std::cout
        // =========================================================
        {
            std::lock_guard<std::mutex> lock(cout_mtx_);

            if (std::isnan(s.value)) {
                // Campione mancante — segnalato esplicitamente a console
                std::cout << s.timestamp << " | "
                          << s.sensor_id << " | "
                          << "[DROPOUT]"
                          << std::endl;
            } else {
                std::cout << s.timestamp << " | "
                          << s.sensor_id << " | "
                          << s.value     << " "
                          << s.unit
                          << std::endl;
            }
        }

        // =========================================================
        // LOG — NaN scritto come stringa vuota nel CSV in modo che
        // pandas lo interpreti automaticamente come NaN alla lettura
        // =========================================================
        const std::string value_str = std::isnan(s.value) ? "" : std::to_string(s.value);

        const std::string line =
            s.timestamp + "," +
            s.sensor_id + "," +
            value_str   + "," +
            s.unit;

        logger_.log(line);

        // Sleep DOPO lettura e log — rispetta il periodo di campionamento
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