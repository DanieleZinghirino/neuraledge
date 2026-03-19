#pragma once

#include <string>
#include <mutex>

class CsvLogger {
    public:
        explicit CsvLogger(const std::string& filename);

        void log(const std::string& line);
        void init();

    private:
        std::string filename_;
        std::mutex mtx_;
};