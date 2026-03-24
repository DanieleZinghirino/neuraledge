#include "factory/SensorFactory.hpp"

#include "signal/TemperatureSignal.hpp"
#include "signal/CurrentSignal.hpp"
#include "signal/SpeedSignal.hpp"

#include "noise/GaussianNoise.hpp"
#include "noise/UniformNoise.hpp"
#include "noise/PinkNoise.hpp"
#include "noise/DriftNoise.hpp"
#include "noise/PeriodicNoise.hpp"
#include "noise/ImpulseNoise.hpp"

#include "fault/SpikeFault.hpp"

// Definizione della variabile statica
std::map<std::string, int> SensorFactory::counters_;

Sensor SensorFactory::createRandom(std::mt19937& gen) {

    // =========================================================
    // DISTRIBUZIONI
    // =========================================================
    std::uniform_int_distribution<int>     dis_signal_type (0,    2   );
    std::uniform_int_distribution<int>     dis_noise_type  (0,    4   );

    std::uniform_real_distribution<double> dis_rate        (1.0,  3.0 );

    // Signal
    std::uniform_real_distribution<double> dis_base        (15.0, 35.0);
    std::uniform_real_distribution<double> dis_amp         (0.5,  3.0 );
    std::uniform_real_distribution<double> dis_freq        (0.01, 0.1 );
    std::uniform_real_distribution<double> dis_v0          (0.0,  10.0);
    std::uniform_real_distribution<double> dis_accel       (0.01, 0.5 );

    // Noise — parametri condivisi tra più tipi
    std::uniform_real_distribution<double> dis_noise_sigma (0.1,  0.6 );
    std::uniform_real_distribution<double> dis_noise_amp   (0.05, 0.3 );
    std::uniform_real_distribution<double> dis_drift_step  (0.001,0.01);
    std::uniform_real_distribution<double> dis_drift_max   (0.5,  2.0 );
    std::uniform_real_distribution<double> dis_impulse_prob(0.01, 0.05);
    std::uniform_real_distribution<double> dis_impulse_sig (1.0,  5.0 );
    std::uniform_int_distribution<int>     dis_periodic_hz (50, 100   );

    // Fault
    std::uniform_real_distribution<double> dis_spike_prob  (0.0,  0.6 );
    std::uniform_real_distribution<double> dis_spike_mag   (0.0,  15.0);

    // =========================================================
    // SEGNALE — il sampling rate viene campionato prima perché
    // PeriodicNoise ne ha bisogno per sincronizzare il tempo interno
    // =========================================================
    const double rate = dis_rate(gen);

    std::unique_ptr<ISignalModel> signal;
    switch (dis_signal_type(gen)) {
        case 0:
            signal = std::make_unique<TemperatureSignal>(
                dis_base(gen), dis_amp(gen), dis_freq(gen));
            break;
        case 1:
            signal = std::make_unique<CurrentSignal>(
                dis_base(gen), dis_amp(gen), dis_freq(gen));
            break;
        case 2:
            signal = std::make_unique<SpeedSignal>(
                dis_v0(gen), dis_accel(gen));
            break;
    }

    // =========================================================
    // ID — costruito chiedendo al segnale il suo typeName,
    // la factory non hardcoda nessuna stringa di tipo
    // =========================================================
    const std::string type      = signal->typeName();
    const std::string sensor_id = type + std::to_string(++counters_[type]);

    // =========================================================
    // RUMORE — scelto casualmente tra i modelli disponibili
    // =========================================================
    std::unique_ptr<INoiseModel> noise;
    switch (dis_noise_type(gen)) {
        case 0:
            // Rumore termico / elettronico generico
            noise = std::make_unique<GaussianNoise>(
                dis_noise_sigma(gen));
            break;
        case 1:
            // Errore di quantizzazione ADC
            noise = std::make_unique<UniformNoise>(
                dis_noise_amp(gen));
            break;
        case 2:
            // Rumore 1/f — sensori MEMS, pressione
            noise = std::make_unique<PinkNoise>(
                dis_noise_sigma(gen));
            break;
        case 3:
            // Deriva termica / invecchiamento
            noise = std::make_unique<DriftNoise>(
                dis_drift_step(gen),
                dis_drift_max(gen));
            break;
        case 4:
            // Interferenza rete elettrica — 50Hz o 100Hz (seconda armonica)
            // Il sampling rate viene passato per sincronizzare il tempo interno
            noise = std::make_unique<PeriodicNoise>(
                dis_noise_amp(gen),
                dis_periodic_hz(gen),
                rate);
            break;

        // NOTA: ImpulseNoise non viene usato da solo perché modella
        // un disturbo sovrapposto ad altro rumore di fondo. Sarà
        // integrabile tramite Chain of Responsibility (pattern futuro)
        // che permetterà di combinare più modelli di rumore in pipeline.
    }

    // =========================================================
    // FAULT
    // =========================================================
    auto fault = std::make_unique<SpikeFault>(
        dis_spike_prob(gen),
        dis_spike_mag(gen));

    // =========================================================
    // COSTRUZIONE FINALE
    // =========================================================
    return Sensor(
        sensor_id,
        rate,
        std::move(signal),
        std::move(noise),
        std::move(fault)
    );
}