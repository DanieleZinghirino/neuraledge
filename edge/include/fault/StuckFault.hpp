#pragma once
#include "IFaultModel.hpp"
#include <random>
#include <optional>

/**
 * @brief Sensore bloccato su un valore fisso.
 *
 * MODELLO FISICO:
 * Il guasto più comune nei sensori industriali: il sensore smette
 * di aggiornarsi e restituisce sempre lo stesso valore. Causato da
 * perdita di connessione, freeze del firmware, o cortocircuito
 * sull'uscita analogica.
 *
 * COMPORTAMENTO:
 * - Con probabilità stuck_prob, il guasto si attiva al primo campione
 *   e rimane attivo per sempre (guasto permanente).
 * - Il valore congelato è il primo valore ricevuto quando il guasto
 *   scatta — simula il sensore che si blocca sull'ultimo valore letto.
 *
 * RICONOSCIMENTO:
 * Una serie temporale con varianza zero per N campioni consecutivi
 * è il segnale diagnostico classico di questo guasto.
 */
class StuckFault : public IFaultModel {
    private:
        double stuck_prob_;   // probabilità di attivazione del guasto

        std::mt19937 gen_;
        std::uniform_real_distribution<double> trigger_dist_;

        bool                  is_stuck_;      // stato: guasto attivo o no
        std::optional<double> stuck_value_;   // valore congelato (vuoto finché non scatta)

    public:
        /**
        * @param stuck_prob  Probabilità che il sensore si blocchi [0, 1].
        *                    Valori tipici: 0.001 - 0.01 (guasto raro).
        */
        explicit StuckFault(double stuck_prob);

        double apply(double value) override;
};