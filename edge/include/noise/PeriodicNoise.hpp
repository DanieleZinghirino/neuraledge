#pragma once
#include "INoiseModel.hpp"
#include <random>

/**
 * @brief Interferenza sinusoidale periodica a frequenza fissa.
 *
 * MODELLO FISICO:
 * In ambienti industriali i cavi dei sensori agiscono da antenne
 * e captano interferenze dalla rete elettrica (50Hz in Europa,
 * 60Hz in America) o da motori elettrici e inverter nelle vicinanze.
 * Il risultato è una sinusoide sovrapposta al segnale utile.
 *
 *   interference(t) = amplitude * sin(2π * frequency * t + phase)
 *
 * Il tempo interno avanza di 1/sampling_rate ad ogni campione,
 * coerentemente con il sampling_rate del Sensor.
 *
 * USO TIPICO:
 * frequency = 50.0  Hz   (rete europea)
 * frequency = 100.0 Hz   (seconda armonica, comune vicino a trasformatori)
 * amplitude = 0.1 - 2.0  (dipende dalla schermatura del cavo)
 */
class PeriodicNoise : public INoiseModel {
    private:
        double amplitude_;      // ampiezza dell'interferenza
        double frequency_;      // frequenza in Hz
        double phase_;          // fase iniziale in radianti
        double sampling_rate_;  // Hz — determina l'avanzamento del tempo interno
        double current_time_;   // stato interno: tempo corrente in secondi

    public:
        /**
        * @param amplitude     Ampiezza picco dell'interferenza.
        * @param frequency     Frequenza in Hz (tipicamente 50 o 60).
        * @param sampling_rate Frequenza di campionamento del sensore in Hz.
        * @param phase         Fase iniziale in radianti (default 0.0).
        */
        PeriodicNoise(double amplitude, double frequency,
                    double sampling_rate, double phase = 0.0);

        double apply() override;
};