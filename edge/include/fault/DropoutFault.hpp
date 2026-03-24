#pragma once
#include "IFaultModel.hpp"
#include <random>
#include <limits>

/**
 * @brief Perdita intermittente del segnale — campioni mancanti.
 *
 * MODELLO FISICO:
 * Connessioni instabili, timeout di comunicazione, o reset del
 * microcontrollore causano l'assenza periodica di dati.
 * In un sistema reale questi campioni arriverebbero come NaN,
 * come valore sentinella (0, -1, fondo scala), o semplicemente
 * non arriverebbero (gap nella serie temporale).
 *
 * IMPLEMENTAZIONE:
 * Usiamo NaN come valore sentinella — è il modo standard in C++
 * per rappresentare un dato mancante in virgola mobile.
 * L'analyzer Python lo gestirà nativamente con pandas (NaN è
 * ignorato da mean(), std(), rolling() per default).
 *
 * MODELLO:
 * Il dropout può essere in due modalità:
 * - RANDOM: ogni campione ha probabilità indipendente di dropout
 * - BURST:  il dropout dura per burst_length campioni consecutivi
 *
 * NOTA SULL'ANALYZER:
 * csv_loader.py deve gestire i NaN — pandas li legge automaticamente
 * se la cella è vuota o contiene "nan".
 */
class DropoutFault : public IFaultModel {
    public:
        enum class Mode { RANDOM, BURST };

    private:
        double dropout_prob_;   // probabilità di dropout per campione
        int    burst_length_;   // durata del burst in campioni (usato in BURST mode)
        Mode   mode_;

        int  remaining_dropout_; // campioni di dropout rimanenti nel burst corrente
        bool in_dropout_;

        std::mt19937 gen_;
        std::uniform_real_distribution<double> trigger_dist_;

    public:
        /**
        * @param dropout_prob  Probabilità di dropout per campione.
        * @param mode          RANDOM (indipendente) o BURST (consecutivo).
        * @param burst_length  Durata del burst in campioni (ignorato in RANDOM).
        */
        DropoutFault(double dropout_prob,
                    Mode   mode         = Mode::RANDOM,
                    int    burst_length = 5);

        double apply(double value) override;
};