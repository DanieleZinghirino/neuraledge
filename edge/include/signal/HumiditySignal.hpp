#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Segnale di umidità relativa con boundary fisici garantiti.
 *
 * MODELLO FISICO:
 * L'umidità relativa è fisicamente limitata a [0%, 100%].
 * Varia lentamente con correlazioni temporali forti — una semplice
 * sinusoide non è realistica perché può produrre valori fuori range.
 *
 * MODELLO MATEMATICO:
 * Proiezione sigmoidale di una sinusoide lenta:
 *
 *   raw(t)  = base + amplitude * sin(2π * freq * t)
 *   H(t)    = 100 / (1 + e^(-k * (raw(t) - 50)))
 *
 * La funzione sigmoide mappa ℝ → (0, 100) con:
 * - valori centrali (vicino a 50%) → risposta quasi lineare
 * - valori estremi → compressione naturale verso i boundary
 * - k controlla la "durezza" della compressione ai bordi
 *
 * Questo è più realistico del semplice clamping perché i valori
 * estremi sono raggiunti asintoticamente, come fisicamente accade.
 *
 * UNITÀ: % (percentuale di umidità relativa)
 */
class HumiditySignal : public ISignalModel {
    private:
        double base_;       // umidità centrale in % (es. 60.0)
        double amplitude_;  // ampiezza dell'oscillazione prima della sigmoide
        double frequency_;  // frequenza dell'oscillazione in Hz
        double k_;          // pendenza della sigmoide (default: 0.1)

    public:
        /**
        * @param base        Umidità media in % [0, 100].
        * @param amplitude   Ampiezza dell'oscillazione (pre-sigmoide).
        * @param frequency   Frequenza in Hz — tipicamente molto bassa (0.001-0.01).
        * @param k           Pendenza sigmoide (default 0.1 — transizione morbida).
        */
        HumiditySignal(double base, double amplitude,
                    double frequency, double k = 0.1);

        double      generate  (double t) const override;
        std::string typeName  ()         const override;
        std::string unit      ()         const override;
};