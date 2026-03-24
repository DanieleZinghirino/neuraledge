#pragma once
#include "ISignalModel.hpp"
#include <random>

/**
 * @brief Segnale di pressione con baseline quasi-statica e transitori.
 *
 * MODELLO FISICO:
 * La pressione in un impianto industriale è tipicamente quasi-statica:
 * varia lentamente attorno a un valore di esercizio, ma è soggetta
 * a transitori rapidi causati da apertura/chiusura di valvole,
 * avvio/stop di pompe, o variazioni di carico.
 *
 * MODELLO MATEMATICO:
 * Tre componenti sovrapposte:
 *   1. Baseline lenta: P₀ + A_slow * sin(2π * f_slow * t)
 *      → oscillazione di processo (es. ciclo di produzione)
 *   2. Transitorio: gradino esponenziale a istante casuale
 *      → P_trans * (1 - e^(-t_rel/τ)) quando attivo
 *   3. La somma è sempre clampata a [0, P_max] per rispettare
 *      i limiti fisici del sistema (pressione non negativa)
 *
 * UNITÀ: bar
 */
class PressureSignal : public ISignalModel {
    private:
        double baseline_;        // pressione di esercizio in bar
        double slow_amplitude_;  // ampiezza oscillazione lenta
        double slow_freq_;       // frequenza oscillazione lenta in Hz

        // Parametri dei transitori
        mutable double transient_prob_;  // probabilità per campione di un transitorio
        mutable double transient_mag_;   // ampiezza del gradino in bar
        mutable double transient_tau_;   // costante di tempo del transitorio in secondi
        mutable double p_max_;           // pressione massima fisicamente ammissibile

        // Stato interno del transitorio corrente
        mutable bool   in_transient_;
        mutable double transient_start_; // istante di inizio del transitorio
        mutable int    transient_sign_;  // +1 (pressurizzazione) o -1 (depressurizzazione)

        mutable std::mt19937 gen_;
        mutable std::uniform_real_distribution<double> trigger_dist_;
        mutable std::uniform_int_distribution<int>     sign_dist_;

    public:
        /**
        * @param baseline        Pressione di esercizio in bar.
        * @param slow_amplitude  Ampiezza dell'oscillazione lenta in bar.
        * @param slow_freq       Frequenza dell'oscillazione lenta in Hz.
        * @param transient_prob  Probabilità per campione di un transitorio.
        * @param transient_mag   Ampiezza del gradino transitorio in bar.
        * @param transient_tau   Costante di tempo del transitorio in secondi.
        * @param p_max           Pressione massima fisicamente ammissibile.
        */
        PressureSignal(double baseline,
                    double slow_amplitude, double slow_freq,
                    double transient_prob, double transient_mag,
                    double transient_tau,  double p_max);

        double      generate  (double t) const override;
        std::string typeName  ()         const override;
        std::string unit      ()         const override;
};