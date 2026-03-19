#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>

namespace utils {

class TimestampUtils {
public:
    // Timestamp leggibile in locale (default: YYYY-MM-DD HH:MM:SS.mmm)
    static std::string nowLocal(const std::string& format = "%Y-%m-%d %H:%M:%S", bool includeMs = true) {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&now_time);

        std::ostringstream oss;
        oss << std::put_time(&local_tm, format.c_str());
        if (includeMs) {
            oss << "." << std::setfill('0') << std::setw(3) << ms.count();
        }
        return oss.str();
    }

    // Timestamp leggibile in UTC
    static std::string nowUTC(const std::string& format = "%Y-%m-%d %H:%M:%S", bool includeMs = true) {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm utc_tm = *std::gmtime(&now_time);

        std::ostringstream oss;
        oss << std::put_time(&utc_tm, format.c_str());
        if (includeMs) {
            oss << "." << std::setfill('0') << std::setw(3) << ms.count();
        }
        return oss.str();
    }

    // Timestamp sicuro per nomi di file (senza caratteri ":")
    static std::string nowFileSafeUTC() {
        std::string ts = nowUTC("%Y-%m-%d_%H-%M-%S", true);
        std::replace(ts.begin(), ts.end(), ':', '-'); // sostituisce eventuali ":" con "-"
        return ts;
    }
};

} // namespace utils