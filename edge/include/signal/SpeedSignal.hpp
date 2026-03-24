#pragma once
#include "ISignalModel.hpp"

/**
 * @brief Segnale di velocità con profilo trapezoidale ciclico.
 *
 * MODELLO FISICO:
 * Un motore o un asse controllato non accelera indefinitamente
 * (il modello precedente era un moto uniformemente accelerato
 * senza limiti — non fisico). In realtà segue un profilo a
 * trapezoide di velocità: accelerazione → crociera → decelerazione → fermo.
 *
 * MODELLO MATEMATICO:
 * Il ciclo ha durata cycle_period_ ed è diviso in 4 fasi:
 *
 *   Fase 1 — Accelerazione [0, t_acc]:
 *     v(t) = v_max * (t_rel / t_acc)
 *
 *   Fase 2 — Crociera [t_acc, t_acc + t_cruise]:
 *     v(t) = v_max
 *
 *   Fase 3 — Decelerazione [t_acc + t_cruise, t_acc + t_cruise + t_dec]:
 *     v(t) = v_max * (1 - (t_rel - t_acc - t_cruise) / t_dec)
 *
 *   Fase 4 — Fermo [t_acc + t_cruise + t_dec, cycle_period]:
 *     v(t) = 0
 *
 * UNITÀ: m/s
 */
class SpeedSignal : public ISignalModel {
private:
    double v_max_;          // velocità massima di crociera (m/s)
    double t_acc_;          // durata della fase di accelerazione (s)
    double t_cruise_;       // durata della fase di crociera (s)
    double t_dec_;          // durata della fase di decelerazione (s)
    double cycle_period_;   // durata totale del ciclo (s) — calcolata in ctor

public:
    /**
     * @param v_max     Velocità massima di crociera (m/s).
     * @param t_acc     Durata accelerazione (s).
     * @param t_cruise  Durata crociera (s).
     * @param t_dec     Durata decelerazione (s).
     * @param t_stop    Durata del fermo a fine ciclo (s).
     */
    SpeedSignal(double v_max,
                double t_acc, double t_cruise,
                double t_dec, double t_stop);

    double      generate  (double t) const override;
    std::string typeName  ()         const override;
    std::string unit      ()         const override;
};