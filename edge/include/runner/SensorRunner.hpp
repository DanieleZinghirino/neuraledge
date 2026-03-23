#pragma once

#include "sensor/Sensor.hpp"
#include "logger/ILogger.hpp"
#include <vector>
#include <thread>
#include <mutex>

class SensorRunner{
    public:
        SensorRunner(std::vector<Sensor>& sensors, ILogger& logger);

        void run();
    
    private:
        void worker(Sensor &sensor);

        std::vector<Sensor>& sensors_;
        ILogger& logger_;           // dipende dall'astrazione, non dal concreto
        std::vector<std::thread> threads_;
        std::mutex cout_mtx_;
};