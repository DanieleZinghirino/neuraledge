#pragma once

#include "sensor/Sensor.hpp"
#include "logger/CsvLogger.hpp"
#include <vector>
#include <thread>

class SensorRunner{
    public:
        SensorRunner(std::vector<Sensor>& sensors, CsvLogger& logger);

        void run();
    
    private:
        void worker(Sensor &sensor);

        std::vector<Sensor>& sensors_;
        CsvLogger& logger_;
        std::vector<std::thread> threads_;
};