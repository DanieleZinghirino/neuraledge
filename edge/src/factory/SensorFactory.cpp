#include "factory/SensorFactory.hpp"

// Signal
#include "signal/CurrentSignal.hpp"
#include "signal/HumiditySignal.hpp"
#include "signal/PressureSignal.hpp"
#include "signal/RampSignal.hpp"
#include "signal/SpeedSignal.hpp"
#include "signal/SquareWaveSignal.hpp"
#include "signal/StepResponseSignal.hpp"
#include "signal/TemperatureSignal.hpp"
#include "signal/VibrationSignal.hpp"

// Noise
#include "noise/DriftNoise.hpp"
#include "noise/GaussianNoise.hpp"
#include "noise/ImpulseNoise.hpp"
#include "noise/PeriodicNoise.hpp"
#include "noise/PinkNoise.hpp"
#include "noise/UniformNoise.hpp"

// Fault
#include "fault/DriftFault.hpp"
#include "fault/DropoutFault.hpp"
#include "fault/NoFault.hpp"
#include "fault/SaturationFault.hpp"
#include "fault/ScalingFault.hpp"
#include "fault/SpikeFault.hpp"
#include "fault/StuckFault.hpp"

#include <algorithm>
#include <cctype>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

std::string normalize(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        if (c == '-' || c == ' ') {
            return static_cast<char>('_');
        }
        return static_cast<char>(std::tolower(c));
    });
    return s;
}

double getRequiredNumber(const config::ModelConfig& cfg, const std::string& key) {
    auto it = cfg.numeric_params.find(key);
    if (it == cfg.numeric_params.end()) {
        throw std::runtime_error(
            "Missing numeric parameter '" + key + "' for model '" + cfg.model + "'"
        );
    }
    return it->second;
}

double getOptionalNumber(const config::ModelConfig& cfg,
                         const std::string& key,
                         double default_value) {
    auto it = cfg.numeric_params.find(key);
    if (it == cfg.numeric_params.end()) {
        return default_value;
    }
    return it->second;
}

std::string getOptionalString(const config::ModelConfig& cfg,
                              const std::string& key,
                              const std::string& default_value) {
    auto it = cfg.string_params.find(key);
    if (it == cfg.string_params.end()) {
        return default_value;
    }
    return it->second;
}

std::unique_ptr<ISignalModel> buildSignal(const config::SensorConfig& cfg) {
    const std::string model = normalize(cfg.signal.model);

    if (model == "temperature") {
        return std::make_unique<TemperatureSignal>(
            getRequiredNumber(cfg.signal, "base"),
            getRequiredNumber(cfg.signal, "tau"),
            getRequiredNumber(cfg.signal, "slow_amp"),
            getRequiredNumber(cfg.signal, "slow_freq"),
            getRequiredNumber(cfg.signal, "fast_amp"),
            getRequiredNumber(cfg.signal, "fast_freq")
        );
    }

    if (model == "current") {
        return std::make_unique<CurrentSignal>(
            getRequiredNumber(cfg.signal, "base"),
            getRequiredNumber(cfg.signal, "fundamental"),
            getRequiredNumber(cfg.signal, "freq"),
            getRequiredNumber(cfg.signal, "thd3_amp"),
            getRequiredNumber(cfg.signal, "thd3_phase"),
            getRequiredNumber(cfg.signal, "thd5_amp"),
            getRequiredNumber(cfg.signal, "thd5_phase"),
            getRequiredNumber(cfg.signal, "load_period"),
            getRequiredNumber(cfg.signal, "delta_i"),
            getRequiredNumber(cfg.signal, "tau_load")
        );
    }

    if (model == "speed") {
        return std::make_unique<SpeedSignal>(
            getRequiredNumber(cfg.signal, "vmax"),
            getRequiredNumber(cfg.signal, "t_acc"),
            getRequiredNumber(cfg.signal, "t_cruise"),
            getRequiredNumber(cfg.signal, "t_dec"),
            getRequiredNumber(cfg.signal, "t_stop")
        );
    }

    if (model == "vibration") {
        std::vector<VibrationSignal::Harmonic> harmonics;
        harmonics.push_back({
            getRequiredNumber(cfg.signal, "amp1"),
            1.0,
            getOptionalNumber(cfg.signal, "phase1", 0.0)
        });
        harmonics.push_back({
            getOptionalNumber(cfg.signal, "amp2", 0.0),
            2.0,
            getOptionalNumber(cfg.signal, "phase2", 0.0)
        });
        harmonics.push_back({
            getOptionalNumber(cfg.signal, "amp3", 0.0),
            3.0,
            getOptionalNumber(cfg.signal, "phase3", 0.0)
        });

        return std::make_unique<VibrationSignal>(
            getRequiredNumber(cfg.signal, "base_freq"),
            std::move(harmonics)
        );
    }

    if (model == "pressure") {
        return std::make_unique<PressureSignal>(
            getRequiredNumber(cfg.signal, "baseline"),
            getRequiredNumber(cfg.signal, "slow_amplitude"),
            getRequiredNumber(cfg.signal, "slow_freq"),
            getRequiredNumber(cfg.signal, "transient_prob"),
            getRequiredNumber(cfg.signal, "transient_mag"),
            getRequiredNumber(cfg.signal, "transient_tau"),
            getRequiredNumber(cfg.signal, "p_max")
        );
    }

    if (model == "humidity") {
        return std::make_unique<HumiditySignal>(
            getRequiredNumber(cfg.signal, "base"),
            getRequiredNumber(cfg.signal, "amplitude"),
            getRequiredNumber(cfg.signal, "frequency")
        );
    }

    if (model == "square_wave") {
        return std::make_unique<SquareWaveSignal>(
            getRequiredNumber(cfg.signal, "period"),
            getRequiredNumber(cfg.signal, "duty_cycle")
        );
    }

    if (model == "ramp") {
        return std::make_unique<RampSignal>(
            getRequiredNumber(cfg.signal, "period"),
            getRequiredNumber(cfg.signal, "low"),
            getRequiredNumber(cfg.signal, "high")
        );
    }

    if (model == "step_response") {
        return std::make_unique<StepResponseSignal>(
            getRequiredNumber(cfg.signal, "high"),
            getRequiredNumber(cfg.signal, "low"),
            getRequiredNumber(cfg.signal, "tau"),
            getRequiredNumber(cfg.signal, "period")
        );
    }

    throw std::runtime_error("Unsupported signal model: " + cfg.signal.model);
}

std::unique_ptr<INoiseModel> buildNoise(const config::SensorConfig& cfg) {
    const std::string model = normalize(cfg.noise.model);

    if (model == "gaussian") {
        return std::make_unique<GaussianNoise>(
            getRequiredNumber(cfg.noise, "sigma")
        );
    }

    if (model == "uniform") {
        return std::make_unique<UniformNoise>(
            getRequiredNumber(cfg.noise, "amplitude")
        );
    }

    if (model == "pink") {
        return std::make_unique<PinkNoise>(
            getRequiredNumber(cfg.noise, "sigma")
        );
    }

    if (model == "drift") {
        return std::make_unique<DriftNoise>(
            getRequiredNumber(cfg.noise, "step_sigma"),
            getRequiredNumber(cfg.noise, "max_drift")
        );
    }

    if (model == "periodic") {
        return std::make_unique<PeriodicNoise>(
            getRequiredNumber(cfg.noise, "amplitude"),
            getRequiredNumber(cfg.noise, "frequency"),
            cfg.sampling_rate,
            getOptionalNumber(cfg.noise, "phase", 0.0)
        );
    }

    if (model == "impulse") {
        return std::make_unique<ImpulseNoise>(
            getRequiredNumber(cfg.noise, "probability"),
            getRequiredNumber(cfg.noise, "impulse_sigma")
        );
    }

    throw std::runtime_error("Unsupported noise model: " + cfg.noise.model);
}

std::unique_ptr<IFaultModel> buildFault(const config::SensorConfig& cfg) {
    const std::string model = normalize(cfg.fault.model);

    if (model == "none" || model == "nofault") {
        return std::make_unique<NoFault>();
    }

    if (model == "spike") {
        return std::make_unique<SpikeFault>(
            getRequiredNumber(cfg.fault, "probability"),
            getRequiredNumber(cfg.fault, "magnitude")
        );
    }

    if (model == "stuck") {
        return std::make_unique<StuckFault>(
            getRequiredNumber(cfg.fault, "stuck_prob")
        );
    }

    if (model == "drift") {
        return std::make_unique<DriftFault>(
            getRequiredNumber(cfg.fault, "activation_prob"),
            getRequiredNumber(cfg.fault, "drift_rate"),
            getRequiredNumber(cfg.fault, "max_drift"),
            getOptionalNumber(cfg.fault, "exponent", 1.5)
        );
    }

    if (model == "scaling") {
        return std::make_unique<ScalingFault>(
            getRequiredNumber(cfg.fault, "scale_factor"),
            getRequiredNumber(cfg.fault, "activation_prob")
        );
    }

    if (model == "saturation") {
        return std::make_unique<SaturationFault>(
            getRequiredNumber(cfg.fault, "lower_bound"),
            getRequiredNumber(cfg.fault, "upper_bound"),
            getRequiredNumber(cfg.fault, "activation_prob")
        );
    }

    if (model == "dropout") {
        const std::string mode_str = normalize(getOptionalString(cfg.fault, "mode", "random"));
        const auto mode = (mode_str == "burst")
            ? DropoutFault::Mode::BURST
            : DropoutFault::Mode::RANDOM;

        return std::make_unique<DropoutFault>(
            getRequiredNumber(cfg.fault, "probability"),
            mode,
            static_cast<int>(getOptionalNumber(cfg.fault, "burst_length", 5.0))
        );
    }

    throw std::runtime_error("Unsupported fault model: " + cfg.fault.model);
}

} // namespace

Sensor SensorFactory::createFromConfig(const config::SensorConfig& cfg, std::mt19937& /*gen*/) {
    auto signal = buildSignal(cfg);
    auto noise  = buildNoise(cfg);
    auto fault  = buildFault(cfg);

    return Sensor(
        cfg.id,
        cfg.sampling_rate,
        std::move(signal),
        std::move(noise),
        std::move(fault)
    );
}

std::vector<Sensor> SensorFactory::createAllFromConfig(
    const std::vector<config::SensorConfig>& configs,
    std::mt19937& gen
) {
    std::vector<Sensor> sensors;
    sensors.reserve(configs.size());

    for (const auto& cfg : configs) {
        sensors.push_back(createFromConfig(cfg, gen));
    }

    return sensors;
}