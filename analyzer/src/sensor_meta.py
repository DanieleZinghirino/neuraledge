from pathlib import Path
from typing import NamedTuple

import yaml


class SensorMeta(NamedTuple):
    sensor_id:   str
    sensor_type: str
    noise_model: str
    fault_model: str
    sampling_rate: float


def load_sensor_meta(config_path: str | Path) -> dict[str, SensorMeta]:
    """
    Legge il file YAML di configurazione e restituisce un dizionario
    sensor_id -> SensorMeta con le informazioni di noise e fault.

    Returns:
        Dict[sensor_id, SensorMeta]

    Raises:
        FileNotFoundError: se il YAML non esiste.
        ValueError:        se la struttura del YAML non è quella attesa.
    """
    config_path = Path(config_path)

    if not config_path.exists():
        raise FileNotFoundError(f"Config YAML non trovato: {config_path}")

    with open(config_path, "r", encoding="utf-8") as f:
        config = yaml.safe_load(f)

    if not isinstance(config, dict) or "sensors" not in config:
        raise ValueError("Il file YAML non contiene la sezione 'sensors'")

    meta: dict[str, SensorMeta] = {}

    for entry in config["sensors"]:
        sensor_id     = entry.get("id",            "unknown")
        sensor_type   = entry.get("type",          "unknown")
        sampling_rate = entry.get("sampling_rate", 1.0)
        noise_model   = entry.get("noise", {}).get("model", "unknown")
        fault_model   = entry.get("fault", {}).get("model", "none")

        meta[sensor_id] = SensorMeta(
            sensor_id     = sensor_id,
            sensor_type   = sensor_type,
            noise_model   = noise_model,
            fault_model   = fault_model,
            sampling_rate = sampling_rate,
        )

    return meta