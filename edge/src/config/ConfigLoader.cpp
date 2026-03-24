#include "config/ConfigLoader.hpp"

#include <yaml-cpp/yaml.h>

#include <sstream>
#include <stdexcept>

namespace config {
namespace {

std::string requireString(const YAML::Node& node, const std::string& key) {
    if (!node[key]) {
        throw std::runtime_error("Missing required string field: " + key);
    }
    return node[key].as<std::string>();
}

double requireDouble(const YAML::Node& node, const std::string& key) {
    if (!node[key]) {
        throw std::runtime_error("Missing required numeric field: " + key);
    }
    return node[key].as<double>();
}

ModelConfig parseModelConfig(const YAML::Node& node, const std::string& section_name) {
    if (!node) {
        throw std::runtime_error("Missing required section: " + section_name);
    }

    ModelConfig cfg;
    cfg.model = requireString(node, "model");

    if (node["params"]) {
        const YAML::Node& params = node["params"];
        if (!params.IsMap()) {
            throw std::runtime_error("Section '" + section_name + ".params' must be a map");
        }

        for (const auto& kv : params) {
            const std::string key = kv.first.as<std::string>();
            const YAML::Node& value = kv.second;

            // Tentativo 1: valore numerico
            try {
                cfg.numeric_params[key] = value.as<double>();
                continue;
            } catch (const YAML::BadConversion&) {
                // non numerico -> proviamo stringa
            }

            // Tentativo 2: valore stringa
            try {
                cfg.string_params[key] = value.as<std::string>();
                continue;
            } catch (const YAML::BadConversion&) {
                // né double né stringa
            }

            throw std::runtime_error(
                "Parameter '" + key + "' in section '" + section_name +
                "' must be either numeric or string"
            );
        }
    }

    return cfg;
}

SensorConfig parseSensorConfig(const YAML::Node& node, std::size_t index) {
    if (!node.IsMap()) {
        throw std::runtime_error("Each sensor entry must be a map. Invalid entry at index " +
                                 std::to_string(index));
    }

    SensorConfig cfg;
    cfg.id            = requireString(node, "id");
    cfg.type          = requireString(node, "type");
    cfg.sampling_rate = requireDouble(node, "sampling_rate");
    cfg.signal        = parseModelConfig(node["signal"], "signal");
    cfg.noise         = parseModelConfig(node["noise"], "noise");
    cfg.fault         = parseModelConfig(node["fault"], "fault");

    if (cfg.sampling_rate <= 0.0) {
        throw std::runtime_error("sampling_rate must be > 0 for sensor '" + cfg.id + "'");
    }

    return cfg;
}

} // namespace

AppConfig ConfigLoader::loadFromYaml(const std::string& path) {
    YAML::Node root = YAML::LoadFile(path);

    if (!root.IsMap()) {
        throw std::runtime_error("Root YAML node must be a map");
    }

    AppConfig config;

    if (!root["simulation"]) {
        throw std::runtime_error("Missing required section: simulation");
    }

    const YAML::Node sim = root["simulation"];
    if (sim["seed"]) {
        config.simulation.seed = sim["seed"].as<unsigned int>();
    }
    if (sim["duration_seconds"]) {
        config.simulation.duration_seconds = sim["duration_seconds"].as<double>();
    }
    if (sim["output_csv"]) {
        config.simulation.output_csv = sim["output_csv"].as<std::string>();
    }

    if (config.simulation.duration_seconds <= 0.0) {
        throw std::runtime_error("simulation.duration_seconds must be > 0");
    }

    if (!root["sensors"]) {
        throw std::runtime_error("Missing required section: sensors");
    }

    const YAML::Node sensors = root["sensors"];
    if (!sensors.IsSequence()) {
        throw std::runtime_error("Section 'sensors' must be a sequence");
    }

    for (std::size_t i = 0; i < sensors.size(); ++i) {
        config.sensors.push_back(parseSensorConfig(sensors[i], i));
    }

    if (config.sensors.empty()) {
        throw std::runtime_error("At least one sensor must be configured");
    }

    return config;
}

} // namespace config