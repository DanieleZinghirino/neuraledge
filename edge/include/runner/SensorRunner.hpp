#pragma once

#include "logger/ILogger.hpp"
#include "sensor/Sensor.hpp"

#include <mutex>
#include <thread>
#include <vector>
#include <unordered_map>
#include <chrono>

class SensorRunner {
public:
    SensorRunner(std::vector<Sensor>& sensors,
                 ILogger& logger,
                 double duration_seconds);

    void run();

private:
    void worker(Sensor& sensor);

    std::vector<Sensor>& sensors_;
    ILogger&             logger_;
    double               duration_seconds_;

    std::vector<std::thread> threads_;
    std::mutex               cout_mtx_;
    
    // Throttle: ultima volta che ogni sensore ha stampato a console
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> last_print_;
    std::mutex last_print_mtx_;

    // Frequenza massima di stampa a console (Hz) — indipendente dal sampling rate
    static constexpr double kMaxConsolePrintHz = 2.0;
};