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
#include "fault/StuckFault.hpp"
#include "fault/DriftFault.hpp"
#include "fault/ScalingFault.hpp"
#include "fault/SaturationFault.hpp"
#include "fault/DropoutFault.hpp"

// Definizione della variabile statica
std::map<std::string, int> SensorFactory::counters_;

Sensor SensorFactory::createRandom(std::mt19937& gen) {

    // =========================================================
    // DISTRIBUZIONI — tipo
    // =========================================================
    std::uniform_int_distribution<int> dis_signal_type(0, 2);
    std::uniform_int_distribution<int> dis_noise_type (0, 4);
    std::uniform_int_distribution<int> dis_fault_type (0, 5);

    // =========================================================
    // DISTRIBUZIONI — signal
    // =========================================================
    std::uniform_real_distribution<double> dis_rate  (1.0,  3.0 );
    std::uniform_real_distribution<double> dis_base  (15.0, 35.0);
    std::uniform_real_distribution<double> dis_amp   (0.5,  3.0 );
    std::uniform_real_distribution<double> dis_freq  (0.01, 0.1 );
    std::uniform_real_distribution<double> dis_v0    (0.0,  10.0);
    std::uniform_real_distribution<double> dis_accel (0.01, 0.5 );

    // =========================================================
    // DISTRIBUZIONI — noise
    // =========================================================
    std::uniform_real_distribution<double> dis_noise_sigma  (0.1,  0.6  );
    std::uniform_real_distribution<double> dis_noise_amp    (0.05, 0.3  );
    std::uniform_real_distribution<double> dis_drift_step   (0.001,0.01 );
    std::uniform_real_distribution<double> dis_drift_max    (0.5,  2.0  );
    std::uniform_real_distribution<double> dis_impulse_prob (0.01, 0.05 );
    std::uniform_real_distribution<double> dis_impulse_sig  (1.0,  5.0  );
    std::uniform_int_distribution<int>     dis_periodic_hz  (50,   100  );

    // =========================================================
    // DISTRIBUZIONI — fault
    // =========================================================
    std::uniform_real_distribution<double> dis_spike_prob   (0.01, 0.1  );
    std::uniform_real_distribution<double> dis_spike_mag    (2.0,  15.0 );
    std::uniform_real_distribution<double> dis_stuck_prob   (0.001,0.01 );
    std::uniform_real_distribution<double> dis_act_prob     (0.001,0.005);
    std::uniform_real_distribution<double> dis_fdrift_rate  (0.01, 0.1  );
    std::uniform_real_distribution<double> dis_fdrift_max   (5.0,  20.0 );
    std::uniform_real_distribution<double> dis_scale        (0.7,  1.3  );
    std::uniform_real_distribution<double> dis_sat_half     (5.0,  15.0 );
    std::uniform_real_distribution<double> dis_dropout_prob (0.02, 0.1  );
    std::uniform_int_distribution<int>     dis_burst_len    (3,    10   );
    std::uniform_int_distribution<int>     dis_dropout_mode (0,    1    );

    // =========================================================
    // SAMPLING RATE — campionato prima perché PeriodicNoise
    // ne ha bisogno per sincronizzare il proprio tempo interno
    // =========================================================
    const double rate = dis_rate(gen);

    // =========================================================
    // SEGNALE
    // =========================================================
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

    // ID costruito interrogando il segnale — factory zero stringhe hardcoded
    const std::string type      = signal->typeName();
    const std::string sensor_id = type + std::to_string(++counters_[type]);

    // =========================================================
    // RUMORE
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
            // Deriva termica lenta — random walk limitato
            noise = std::make_unique<DriftNoise>(
                dis_drift_step(gen),
                dis_drift_max(gen));
            break;
        case 4:
            // Interferenza rete elettrica — 50Hz o 100Hz (seconda armonica).
            // rate passato per sincronizzare il tempo interno del modello
            // con il periodo di campionamento reale del sensore.
            noise = std::make_unique<PeriodicNoise>(
                dis_noise_amp(gen),
                static_cast<double>(dis_periodic_hz(gen)),
                rate);
            break;

        // NOTA: ImpulseNoise è escluso dalla selezione casuale singola
        // perché modella un disturbo da sovrapporre ad altro rumore di fondo.
        // Sarà integrabile tramite Chain of Responsibility (pattern futuro).
    }

    // =========================================================
    // FAULT
    // =========================================================
    std::unique_ptr<IFaultModel> fault;
    switch (dis_fault_type(gen)) {
        case 0:
            // Picco impulsivo positivo casuale
            fault = std::make_unique<SpikeFault>(
                dis_spike_prob(gen),
                dis_spike_mag(gen));
            break;
        case 1:
            // Sensore bloccato sull'ultimo valore letto
            fault = std::make_unique<StuckFault>(
                dis_stuck_prob(gen));
            break;
        case 2:
            // Deriva progressiva e accelerante — degrado fisico irreversibile
            fault = std::make_unique<DriftFault>(
                dis_act_prob(gen),
                dis_fdrift_rate(gen),
                dis_fdrift_max(gen));
            break;
        case 3:
            // Errore di guadagno fisso — calibrazione perduta.
            // dis_scale in [0.7, 1.3]: escluso 1.0 esatto (sarebbe NoFault)
            fault = std::make_unique<ScalingFault>(
                dis_scale(gen),
                dis_act_prob(gen));
            break;
        case 4: {
            // Saturazione ai limiti del range di misura.
            // I bounds sono simmetrici attorno a un centro casuale
            // coerente con il range del segnale (dis_base).
            const double center = dis_base(gen);
            const double half   = dis_sat_half(gen);
            fault = std::make_unique<SaturationFault>(
                center - half,
                center + half,
                dis_act_prob(gen));
            break;
        }
        case 5:
            // Perdita intermittente del segnale — campioni mancanti (NaN).
            // La modalità è scelta casualmente: RANDOM o BURST.
            fault = std::make_unique<DropoutFault>(
                dis_dropout_prob(gen),
                dis_dropout_mode(gen) == 0
                    ? DropoutFault::Mode::RANDOM
                    : DropoutFault::Mode::BURST,
                dis_burst_len(gen));
            break;
    }

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