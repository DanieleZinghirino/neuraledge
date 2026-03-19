#include "logger/CsvLogger.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <iostream>

CsvLogger::CsvLogger(const std::string& filename) 
    : filename_(filename) {}

void CsvLogger::init() {
    std::lock_guard<std::mutex> lock(mtx_);

    std::filesystem::path file_path(filename_);

    // Se esiste già lo rimuovo
    if (std::filesystem::exists(file_path) && std::filesystem::is_directory(file_path)) {
        std::filesystem::remove_all(file_path);
    }

    std::filesystem::create_directories(file_path.parent_path());

    std::ofstream ofs(file_path, std::ios::out | std::ios::trunc);

    if (!ofs.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename_);
    }

    // Scrivo header CSV
    ofs << "timestamp,sensor_id,value\n";
}

void CsvLogger::log(const std::string& line) {
    std::lock_guard<std::mutex> lock(mtx_);

    std::ofstream ofs(filename_, std::ios::app);
    if (!ofs.is_open()) {
        throw std::runtime_error("Cannot open file for logging: " + filename_);
    }

    ofs << line << std::endl;
}