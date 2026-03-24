#include "factory/SensorFactory.hpp"

// Signal
#include "signal/TemperatureSignal.hpp"
#include "signal/CurrentSignal.hpp"
#include "signal/SpeedSignal.hpp"
#include "signal/VibrationSignal.hpp"
#include "signal/PressureSignal.hpp"
#include "signal/HumiditySignal.hpp"
#include "signal/SquareWaveSignal.hpp"
#include "signal/RampSignal.hpp"
#include "signal/StepResponseSignal.hpp"

// Noise
#include "noise/GaussianNoise.hpp"
#include "noise/UniformNoise.hpp"
#include "noise/PinkNoise.hpp"
#include "noise/DriftNoise.hpp"
#include "noise/PeriodicNoise.hpp"

// Fault
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
    std::uniform_int_distribution<int> dis_signal_type(0, 8);
    std::uniform_int_distribution<int> dis_noise_type (0, 4);
    std::uniform_int_distribution<int> dis_fault_type (0, 5);

    // =========================================================
    // DISTRIBUZIONI — comuni a più segnali
    // =========================================================
    std::uniform_real_distribution<double> dis_rate      (1.0,   3.0  );
    std::uniform_real_distribution<double> dis_base      (15.0,  35.0 );
    std::uniform_real_distribution<double> dis_amp       (0.5,   3.0  );
    std::uniform_real_distribution<double> dis_freq      (0.01,  0.1  );

    // =========================================================
    // DISTRIBUZIONI — TemperatureSignal
    // =========================================================
    std::uniform_real_distribution<double> dis_temp_tau      (10.0,  60.0 );
    std::uniform_real_distribution<double> dis_temp_slow_amp (1.0,   5.0  );
    std::uniform_real_distribution<double> dis_temp_slow_freq(0.001, 0.01 );
    std::uniform_real_distribution<double> dis_temp_fast_amp (0.05,  0.3  );
    std::uniform_real_distribution<double> dis_temp_fast_freq(0.5,   2.0  );

    // =========================================================
    // DISTRIBUZIONI — CurrentSignal
    // =========================================================
    std::uniform_real_distribution<double> dis_curr_base      (1.0,  10.0 );
    std::uniform_real_distribution<double> dis_curr_fund      (5.0,  20.0 );
    std::uniform_real_distribution<double> dis_curr_thd_amp   (0.3,  2.0  );
    std::uniform_real_distribution<double> dis_curr_phase     (0.0,  M_PI );
    std::uniform_real_distribution<double> dis_curr_load_per  (10.0, 60.0 );
    std::uniform_real_distribution<double> dis_curr_delta     (1.0,  5.0  );
    std::uniform_real_distribution<double> dis_curr_tau_load  (0.5,  3.0  );

    // =========================================================
    // DISTRIBUZIONI — SpeedSignal
    // =========================================================
    std::uniform_real_distribution<double> dis_speed_vmax   (0.5,  10.0 );
    std::uniform_real_distribution<double> dis_speed_tacc   (2.0,  8.0  );
    std::uniform_real_distribution<double> dis_speed_cruise (5.0,  20.0 );
    std::uniform_real_distribution<double> dis_speed_tdec   (2.0,  8.0  );
    std::uniform_real_distribution<double> dis_speed_tstop  (1.0,  5.0  );

    // =========================================================
    // DISTRIBUZIONI — VibrationSignal
    // =========================================================
    std::uniform_real_distribution<double> dis_vib_freq(10.0, 100.0);
    std::uniform_real_distribution<double> dis_vib_amp (0.1,  2.0  );

    // =========================================================
    // DISTRIBUZIONI — PressureSignal
    // =========================================================
    std::uniform_real_distribution<double> dis_pres_base(2.0,  8.0  );
    std::uniform_real_distribution<double> dis_pres_amp (0.2,  1.0  );
    std::uniform_real_distribution<double> dis_pres_tau (1.0,  5.0  );

    // =========================================================
    // DISTRIBUZIONI — HumiditySignal
    // =========================================================
    std::uniform_real_distribution<double> dis_humi_base (40.0, 70.0 );
    std::uniform_real_distribution<double> dis_humi_amp  (10.0, 30.0 );
    std::uniform_real_distribution<double> dis_humi_freq (0.001,0.005);

    // =========================================================
    // DISTRIBUZIONI — SquareWaveSignal
    // =========================================================
    std::uniform_real_distribution<double> dis_sq_period(5.0,  30.0 );
    std::uniform_real_distribution<double> dis_sq_duty  (0.2,  0.8  );

    // =========================================================
    // DISTRIBUZIONI — RampSignal
    // =========================================================
    std::uniform_real_distribution<double> dis_ramp_period(10.0, 60.0);

    // =========================================================
    // DISTRIBUZIONI — StepResponseSignal
    // =========================================================
    std::uniform_real_distribution<double> dis_step_high(60.0, 90.0 );
    std::uniform_real_distribution<double> dis_step_low (20.0, 40.0 );
    std::uniform_real_distribution<double> dis_step_tau (2.0,  10.0 );
    std::uniform_real_distribution<double> dis_step_per (15.0, 45.0 );

    // =========================================================
    // DISTRIBUZIONI — Noise
    // =========================================================
    std::uniform_real_distribution<double> dis_noise_sigma (0.1,  0.6  );
    std::uniform_real_distribution<double> dis_noise_amp   (0.05, 0.3  );
    std::uniform_real_distribution<double> dis_drift_step  (0.001,0.01 );
    std::uniform_real_distribution<double> dis_drift_max   (0.5,  2.0  );
    std::uniform_int_distribution<int>     dis_periodic_hz (50,   100  );

    // =========================================================
    // DISTRIBUZIONI — Fault
    // =========================================================
    std::uniform_real_distribution<double> dis_spike_prob  (0.01, 0.1  );
    std::uniform_real_distribution<double> dis_spike_mag   (2.0,  15.0 );
    std::uniform_real_distribution<double> dis_stuck_prob  (0.001,0.01 );
    std::uniform_real_distribution<double> dis_act_prob    (0.001,0.005);
    std::uniform_real_distribution<double> dis_fdrift_rate (0.01, 0.1  );
    std::uniform_real_distribution<double> dis_fdrift_max  (5.0,  20.0 );
    std::uniform_real_distribution<double> dis_scale       (0.7,  1.3  );
    std::uniform_real_distribution<double> dis_sat_half    (5.0,  15.0 );
    std::uniform_real_distribution<double> dis_dropout_prob(0.02, 0.1  );
    std::uniform_int_distribution<int>     dis_burst_len   (3,    10   );
    std::uniform_int_distribution<int>     dis_dropout_mode(0,    1    );

    // =========================================================
    // SAMPLING RATE — campionato prima del segnale perché
    // PeriodicNoise ne ha bisogno per sincronizzare il tempo interno
    // =========================================================
    const double rate = dis_rate(gen);

    // =========================================================
    // SEGNALE
    // =========================================================
    std::unique_ptr<ISignalModel> signal;
    switch (dis_signal_type(gen)) {
        case 0:
            // Temperatura con warm-up + ciclo lento + perturbazione termoregolatore
            signal = std::make_unique<TemperatureSignal>(
                dis_base(gen),
                dis_temp_tau(gen),
                dis_temp_slow_amp(gen),  dis_temp_slow_freq(gen),
                dis_temp_fast_amp(gen),  dis_temp_fast_freq(gen));
            break;

        case 1:
            // Corrente con armoniche di distorsione (THD) + transitori di carico
            signal = std::make_unique<CurrentSignal>(
                dis_curr_base(gen),
                dis_curr_fund(gen),
                50.0,                             // frequenza di rete europea
                dis_curr_thd_amp(gen),  dis_curr_phase(gen),   // 3ª armonica
                dis_curr_thd_amp(gen) * 0.5, dis_curr_phase(gen),  // 5ª armonica
                dis_curr_load_per(gen),
                dis_curr_delta(gen),
                dis_curr_tau_load(gen));
            break;

        case 2:
            // Velocità con profilo trapezoidale ciclico (fisicamente limitato)
            signal = std::make_unique<SpeedSignal>(
                dis_speed_vmax(gen),
                dis_speed_tacc(gen),
                dis_speed_cruise(gen),
                dis_speed_tdec(gen),
                dis_speed_tstop(gen));
            break;

        case 3:
            // Vibrazione con 3 armoniche: fondamentale + 2x + 3x
            signal = std::make_unique<VibrationSignal>(
                dis_vib_freq(gen),
                std::vector<VibrationSignal::Harmonic>{
                    { dis_vib_amp(gen),              1.0, 0.0  },
                    { dis_vib_amp(gen) * 0.5,        2.0, 0.3  },
                    { dis_vib_amp(gen) * 0.25,       3.0, 0.6  }
                });
            break;

        case 4:
            // Pressione quasi-statica con transitori di valvola/pompa
            signal = std::make_unique<PressureSignal>(
                dis_pres_base(gen),
                dis_pres_amp(gen),
                dis_freq(gen),
                0.002,                        // prob. transitorio per campione
                dis_pres_amp(gen) * 2.0,      // magnitudine gradino
                dis_pres_tau(gen),
                15.0);                        // pressione massima in bar
            break;

        case 5:
            // Umidità con boundary fisici garantiti dalla sigmoide
            signal = std::make_unique<HumiditySignal>(
                dis_humi_base(gen),
                dis_humi_amp(gen),
                dis_humi_freq(gen));
            break;

        case 6:
            // Onda quadra — attuatori on/off (pompe, valvole, riscaldatori)
            signal = std::make_unique<SquareWaveSignal>(
                dis_sq_period(gen),
                dis_sq_duty(gen));
            break;

        case 7:
            // Rampa periodica — riempimento serbatoio / processo a batch
            signal = std::make_unique<RampSignal>(
                dis_ramp_period(gen), 0.0, 100.0);
            break;

        case 8:
            // Risposta esponenziale a gradini — sistemi con inerzia termica
            signal = std::make_unique<StepResponseSignal>(
                dis_step_high(gen),
                dis_step_low(gen),
                dis_step_tau(gen),
                dis_step_per(gen));
            break;
    }

    // ID costruito interrogando il segnale — zero stringhe hardcoded
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
            // Rumore 1/f — sensori MEMS e di pressione
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
            // Interferenza rete elettrica — 50Hz o 100Hz.
            // rate passato per sincronizzare il tempo interno
            // del modello con il periodo reale di campionamento.
            noise = std::make_unique<PeriodicNoise>(
                dis_noise_amp(gen),
                static_cast<double>(dis_periodic_hz(gen)),
                rate);
            break;
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
            // Errore di guadagno — calibrazione perduta.
            // dis_scale in [0.7, 1.3]: evita 1.0 esatto (sarebbe NoFault)
            fault = std::make_unique<ScalingFault>(
                dis_scale(gen),
                dis_act_prob(gen));
            break;
        case 4: {
            // Saturazione ai limiti del range di misura.
            // Bounds simmetrici attorno a un centro coerente col segnale.
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
            // Modalità scelta casualmente: RANDOM o BURST.
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