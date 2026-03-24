#pragma once

#include "logger/ILogger.hpp"
#include "sensor/Sensor.hpp"

#include <mutex>
#include <thread>
#include <vector>

class SensorRunner {
public:
    SensorRunner(std::vector<Sensor>& sensors,
                 ILogger& logger,
                 double duration_seconds);

    void run();

private:
    void worker(Sensor& sensor);

    std::vector<Sensor>& sensors_;
    ILogger& logger_;
    double duration_seconds_;

    std::vector<std::thread> threads_;
    std::mutex cout_mtx_;
};