#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Segnale a rampa periodica — riempimento/svuotamento ciclico.
 *
 * MODELLO FISICO:
 * Modella processi a batch ciclici: riempimento di serbatoi,
 * avanzamento di nastri trasportatori, processi di carica/scarica.
 * Il segnale cresce linearmente fino al massimo (fase di riempimento),
 * poi cade istantaneamente a zero (reset del batch) e ricomincia.
 *
 * MODELLO MATEMATICO:
 *   phase = fmod(t, period) / period  → posizione nel ciclo [0, 1)
 *   R(t)  = low + (high - low) * phase
 *
 * Il reset istantaneo a fine ciclo è intenzionale: modella lo
 * svuotamento rapido del serbatoio o il reset del processo.
 * Se si preferisce un reset graduale, si può usare un SquareWaveSignal
 * con un RampSignal in cascata (Chain of Responsibility, futuro).
 *
 * UNITÀ: % (livello percentuale, generalizzabile)
 */
class RampSignal : public ISignalModel {
    private:
        double period_;  // durata di un ciclo completo in secondi
        double low_;     // valore minimo (inizio rampa)
        double high_;    // valore massimo (fine rampa, prima del reset)

    public:
        /**
        * @param period  Durata del ciclo completo in secondi.
        * @param low     Valore minimo della rampa.
        * @param high    Valore massimo della rampa.
        */
        RampSignal(double period, double low, double high);

        double      generate  (double t) const override;
        std::string typeName  ()         const override;
        std::string unit      ()         const override;
};