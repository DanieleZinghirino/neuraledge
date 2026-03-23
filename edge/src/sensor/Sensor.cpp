#include "sensor/Sensor.hpp"
#include "utils/TimestampUtils.hpp"

Sensor::Sensor(std::string id, 
               double rate,
               std::unique_ptr<ISignalModel> signal,
               std::unique_ptr<INoiseModel> noise,
               std::unique_ptr<IFaultModel> fault)
    : id_(std::move(id)), 
      sampling_rate_(rate), 
      current_time_(0.0),
      signal_(std::move(signal)), 
      noise_(std::move(noise)), 
      fault_(std::move(fault)) {
        unit_ = signal_->unit();
      }

/**
 * PIPELINE:
 * 1. Genera segnale ideale 
 * 2. Aggiunge rumore al segnale
 * 3. Applica fault al segnale rumoroso
 */

Sample Sensor::read() {
    // 0. Ottiene timestamp attuale
    auto timestamp = utils::TimestampUtils::nowLocal();
    
    // 1. Genera segnale ideale
    double ideal_signal = signal_->generate(current_time_);

    // 2. Aggiunge rumore al segnale
    double noisy_signal = ideal_signal + noise_->apply();

    // 3. Applica fault al segnale rumoroso
    double final_value = fault_->apply(noisy_signal);

    // Campione finale
    Sample sample {
        id_,
        final_value,
        timestamp,
        unit_
    };

    // Aggiorna il tempo per la prossima lettura
    current_time_ += 1.0 / sampling_rate_;

    return sample;
}