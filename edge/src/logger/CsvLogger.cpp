#include "logger/CsvLogger.hpp"
#include <filesystem>
#include <stdexcept>

CsvLogger::CsvLogger(const std::string& filename) 
    : filename_(filename) {}

CsvLogger::~CsvLogger() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (ofs_.is_open()) {
        ofs_.close();
    }
}

void CsvLogger::init() {
    std::lock_guard<std::mutex> lock(mtx_);

    std::filesystem::path file_path(filename_);

    if (std::filesystem::exists(file_path) && std::filesystem::is_directory(file_path)) {
        std::filesystem::remove_all(file_path);
    }

    std::filesystem::create_directories(file_path.parent_path());

    // Apre il file una sola volta — rimane aperto fino al distruttore
    ofs_.open(file_path, std::ios::out | std::ios::trunc);

    if (!ofs_.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename_);
    }

    ofs_ << "timestamp,sensor_id,value\n";
    ofs_.flush();
}

void CsvLogger::log(const std::string& line) {
    std::lock_guard<std::mutex> lock(mtx_);

    if (!ofs_.is_open()) {
        throw std::runtime_error("Logger not initialized. Call init() before log().");
    }

    ofs_ << line << "\n";
    ofs_.flush();
}