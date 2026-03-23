#include "factory/SensorFactory.hpp"

#include "signal/TemperatureSignal.hpp"
#include "signal/CurrentSignal.hpp"
#include "signal/SpeedSignal.hpp"
#include "noise/GaussianNoise.hpp"
#include "fault/SpikeFault.hpp"

// Definizione della variabile statica (obbligatoria fuori dalla classe)
std::map<std::string, int> SensorFactory::counters_;

Sensor SensorFactory::createRandom(std::mt19937& gen) {

    // =========================================================
    // DISTRIBUZIONI
    // =========================================================
    std::uniform_int_distribution<int>     dis_type (0,    2   );
    std::uniform_real_distribution<double> dis_rate (1.0,  3.0 );
    std::uniform_real_distribution<double> dis_base (15.0, 35.0);
    std::uniform_real_distribution<double> dis_amp  (0.5,  3.0 );
    std::uniform_real_distribution<double> dis_freq (0.01, 0.1 );
    std::uniform_real_distribution<double> dis_noise(0.1,  0.6 );
    std::uniform_real_distribution<double> dis_prob (0.0,  0.6 );
    std::uniform_real_distribution<double> dis_spike(0.0,  15.0);
    std::uniform_real_distribution<double> dis_v0   (0.0,  10.0);
    std::uniform_real_distribution<double> dis_accel(0.01, 0.5 );

    // =========================================================
    // SCELTA CASUALE DEL TIPO + COSTRUZIONE DEL SEGNALE
    // =========================================================
    std::unique_ptr<ISignalModel> signal;
    std::string sensor_id;

    switch (dis_type(gen)) {
        case 0:
            sensor_id = "temp"  + std::to_string(++counters_["temp"]);
            signal = std::make_unique<TemperatureSignal>(
                dis_base(gen), dis_amp(gen), dis_freq(gen));
            break;
        case 1:
            sensor_id = "curr"  + std::to_string(++counters_["curr"]);
            signal = std::make_unique<CurrentSignal>(
                dis_base(gen), dis_amp(gen), dis_freq(gen));
            break;
        case 2:
            sensor_id = "speed" + std::to_string(++counters_["speed"]);
            signal = std::make_unique<SpeedSignal>(
                dis_v0(gen), dis_accel(gen));
            break;
    }

    // =========================================================
    // RUMORE E FAULT — uguali per tutti i tipi di sensore
    // =========================================================
    return Sensor(
        sensor_id,
        dis_rate(gen),
        std::move(signal),
        std::make_unique<GaussianNoise>(dis_noise(gen)),
        std::make_unique<SpikeFault>(dis_prob(gen), dis_spike(gen))
    );
}