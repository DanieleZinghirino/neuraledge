#pragma once

#include "ILogger.hpp"
#include <string>
#include <mutex>
#include <fstream>

/**
 * @brief Logger concreto che scrive su file CSV.
 * Implementa l'interfaccia ILogger, gestendo un file di output che rimane aperto per tutta la durata del logger.
 * Utilizza un mutex per garantire la sicurezza in caso di accessi concorrenti da più thread.
 */
class CsvLogger : public ILogger {
    public:
        explicit CsvLogger(const std::string& filename);
        ~CsvLogger();

        void init() override;
        void log(const std::string& line) override;

    private:
        std::string filename_;
        std::mutex mtx_;
        std::ofstream ofs_;  // file tenuto aperto per tutta la vita del logger
};