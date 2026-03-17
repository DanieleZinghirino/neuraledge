#include "sensor/Sensor.hpp"
#include "signal/TemperatureSignal.hpp"
#include "noise/GaussianNoise.hpp"
#include "fault/SpikeFault.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <memory>

// Generatore di numeri casuali per parametri dei sensori
std::random_device rd;
std::mt19937 gen(rd());

// Mutex per sincronizzazione
std::mutex count_mtx;
std::mutex log_mtx;

// Funzione di log su file CSV
void log_sensor(const std::string& filename, const std::string &line){
    std::lock_guard<std::mutex> lock(log_mtx);
    std::ofstream ofs(filename, std::ios::app);
    ofs << line << std::endl;
}

// Funzione di lavoro per ogni sensore
void work(Sensor &sensor, const std::string& log_file) {
    std::uniform_real_distribution<double> sleep_dist(500, 2000); // millisecondi

    for (int i = 0; i < 100; ++i) {
        auto s = sensor.read();

        // Sleep casuale per simulare tempi di lettura variabili
        double sleep_time = sleep_dist(gen);
        std::this_thread::sleep_for(std::chrono::milliseconds((int)sleep_time));

        // Stampa thread-safe su console
        {
            std::lock_guard<std::mutex> lock(count_mtx);
            std::cout << "Timestamp: " << s.timestamp << 
                    " || Sensor ID: " << s.sensor_id <<
                    " || Value: " << s.value << "°C" << std::endl;
        }

        // Log su file CSV
        std::string log_line = s.timestamp + "," + s.sensor_id + "," + std::to_string(s.value);
        log_sensor(log_file, log_line);
    }
}

int main() {
    // Inserimento numero di sensori da parte dell'utente
    std::cout << "Insert number of sensors: ";
    int num_sensors;
    std::cin >> num_sensors; 

    // Definizione sensori e threads
    std::vector<Sensor> sensors;
    std::vector<std::thread> threads;

    // Generazione parametri casuali per i sensori
    std::uniform_real_distribution<double> dis_rate(1.0, 3.0);
    std::uniform_real_distribution<double> dis_temp(15.0, 35.0);
    std::uniform_real_distribution<double> dis_amp(0.5, 3.0);
    std::uniform_real_distribution<double> dis_freq(0.01, 0.1);
    std::uniform_real_distribution<double> dis_noise(0.1, 0.6);
    std::uniform_real_distribution<double> dis_prob(0.0, 0.6);
    std::uniform_real_distribution<double> dis_spike(0.0, 15.0);

    // Definizione file di log
    std::string log_file = "../output/sensor_data.csv";
    // Scrive header CSV una sola volta
    {
        std::lock_guard<std::mutex> lock(log_mtx);
        std::ofstream ofs(log_file);
        ofs << "timestamp,sensor_id,value" << std::endl; // header CSV
    }

    // Creazione sensori con parametri casuali
    for(int i = 0; i < num_sensors; ++i) {
        double rate = dis_rate(gen);
        double temp = dis_temp(gen);
        double amp = dis_amp(gen);
        double freq = dis_freq(gen);
        double noise = dis_noise(gen);
        double prob = dis_prob(gen);
        double spike = dis_spike(gen);

        std::string sensor_id = "temp" + std::to_string(i+1);
        Sensor sensor(sensor_id, rate, 
                      std::make_unique<TemperatureSignal>(temp, amp, freq), 
                      std::make_unique<GaussianNoise>(noise), 
                      std::make_unique<SpikeFault>(prob, spike));
        sensors.push_back(std::move(sensor));
    }

    // Avvio threads per ogni sensore
    for(auto &sensor : sensors) {
        threads.emplace_back(work, std::ref(sensor), log_file);
    }

    // Attesa completamento threads
    for (auto &t : threads) {
        t.join();
    }

    // Fine programma
    std::cout << "All threads completed." << std::endl;
    return 0;
}