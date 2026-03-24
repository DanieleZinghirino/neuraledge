#pragma once
#include "ISignalModel.hpp"
#include <vector>

/**
 * @brief Segnale di vibrazione multi-armonica.
 *
 * MODELLO FISICO:
 * I macchinari rotanti (motori, pompe, compressori) producono
 * vibrazioni composte da più armoniche della frequenza di rotazione.
 * Ogni armonica ha ampiezza e fase proprie — la loro distribuzione
 * relativa è diagnostica di specifici guasti meccanici:
 *
 *   1x  → sbilanciamento del rotore
 *   2x  → disallineamento dell'albero
 *   3x+ → usura degli ingranaggi
 *
 * MODELLO MATEMATICO:
 *   V(t) = Σ Aᵢ * sin(2π * i * f₀ * t + φᵢ)
 *
 * dove f₀ è la frequenza fondamentale (Hz) e i è l'indice dell'armonica.
 * Ogni armonica è configurabile tramite la struct Harmonic.
 *
 * UNITÀ: mm/s (velocità di vibrazione, standard ISO 10816)
 */
class VibrationSignal : public ISignalModel {
    public:
        /**
        * @brief Singola componente armonica del segnale.
        */
        struct Harmonic {
            double amplitude;   // ampiezza in mm/s
            double multiplier;  // multiplo della frequenza fondamentale (1.0, 2.0, 3.0...)
            double phase;       // fase iniziale in radianti
        };

    private:
        double                  base_freq_;   // frequenza fondamentale in Hz
        std::vector<Harmonic>   harmonics_;   // lista delle componenti armoniche

    public:
        /**
        * @param base_freq   Frequenza fondamentale in Hz (es. 50/60 per motori a rete).
        * @param harmonics   Lista delle componenti armoniche da sommare.
        */
        VibrationSignal(double base_freq, std::vector<Harmonic> harmonics);

        double      generate  (double t) const override;
        std::string typeName  ()         const override;
        std::string unit      ()         const override;
};