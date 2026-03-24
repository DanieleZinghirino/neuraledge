#include "config/ConfigLoader.hpp"
#include "factory/SensorFactory.hpp"
#include "logger/CsvLogger.hpp"
#include "runner/SensorRunner.hpp"

#include <exception>
#include <iostream>
#include <random>
#include <vector>

int main(int argc, char** argv) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: ./edge <path-to-config.yaml>\n";
            return 1;
        }

        const std::string config_path = argv[1];

        // 1. Caricamento configurazione
        const config::AppConfig app_config =
            config::ConfigLoader::loadFromYaml(config_path);

        // 2. PRNG riproducibile
        std::mt19937 gen(app_config.simulation.seed);

        // 3. Logger
        CsvLogger logger(app_config.simulation.output_csv);
        logger.init();

        // 4. Sensori costruiti da configurazione
        std::vector<Sensor> sensors =
            SensorFactory::createAllFromConfig(app_config.sensors, gen);

        // 5. Runner
        SensorRunner runner(
            sensors,
            logger,
            app_config.simulation.duration_seconds
        );

        runner.run();

        std::cout << "Simulation completed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 2;
    }
}