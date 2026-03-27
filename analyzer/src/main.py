from pathlib import Path
import sys
import yaml

from sensor_meta import load_sensor_meta
from csv_loader import load_csv
from timeseries import pivot_timeseries, compute_derivative, compute_stats
from plots import plot_per_sensor


def resolve_workspace_root() -> Path:
    """
    Risale fino alla root del workspace.
    """
    current_file = Path(__file__).resolve()

    try:
        workspace_root = current_file.parents[2]
    except IndexError:
        raise RuntimeError("Impossibile determinare la workspace root")

    return workspace_root


def resolve_config_path(workspace_root: Path, argv: list[str]) -> Path:
    """
    Determina il path del file YAML di configurazione.

    Priorità:
    1. argomento CLI
    2. default: edge/config/simulation.yaml
    """
    if len(argv) >= 2:
        config_path = Path(argv[1])

        if not config_path.is_absolute():
            config_path = (workspace_root / config_path).resolve()
    else:
        config_path = (workspace_root / "edge/config/simulation.yaml").resolve()

    if not config_path.exists():
        raise FileNotFoundError(
            f"Config YAML non trovato: {config_path}\n"
            "Passa un file YAML valido oppure assicurati che esista il default."
        )

    return config_path


def resolve_csv_path_from_config(workspace_root: Path, config_path: Path) -> Path:
    """
    Legge il file YAML e ricava simulation.output_csv.
    """
    try:
        with open(config_path, "r", encoding="utf-8") as f:
            config = yaml.safe_load(f)
    except Exception as e:
        raise RuntimeError(f"Errore durante la lettura del file YAML {config_path}: {e}")

    if not isinstance(config, dict):
        raise RuntimeError(f"Il file YAML {config_path} non contiene una mappa valida")

    simulation = config.get("simulation")
    if not isinstance(simulation, dict):
        raise RuntimeError(
            f"Nel file YAML {config_path} manca la sezione 'simulation'"
        )

    output_csv = simulation.get("output_csv")
    if not isinstance(output_csv, str) or not output_csv.strip():
        raise RuntimeError(
            f"Nel file YAML {config_path} manca 'simulation.output_csv' oppure è vuoto"
        )

    csv_path = Path(output_csv)

    if not csv_path.is_absolute():
        # Il path nel YAML è pensato rispetto a edge/build
        edge_build_dir = workspace_root / "edge/build"
        csv_path = (edge_build_dir / csv_path).resolve()

    if not csv_path.exists():
        raise FileNotFoundError(
            f"CSV non trovato in: {csv_path}\n"
            f"Ricavato da simulation.output_csv nel file: {config_path}\n"
            "Assicurati che il programma C++ abbia generato il file."
        )

    return csv_path


def main():
    print("     === NeuralEdge Analyzer ===")

    # =========================
    # 1. Path resolution
    # =========================
    workspace_root = resolve_workspace_root()
    config_path = resolve_config_path(workspace_root, sys.argv)
    csv_path = resolve_csv_path_from_config(workspace_root, config_path)

    print(f"[INFO] Workspace   : {workspace_root}")
    print(f"[INFO] Config path : {config_path}")
    print(f"[INFO] CSV path    : {csv_path}")

    # =========================
    # 2. Load data
    # =========================
    try:
        df = load_csv(csv_path)
    except Exception as e:
        print(f"[ERROR] Errore nel caricamento CSV: {e}")
        sys.exit(1)

    if df.empty:
        print("[WARNING] DataFrame vuoto")
        sys.exit(0)

    print(f"[INFO] Campioni caricati: {len(df)}")

    # =========================
    # 3. Statistics
    # =========================
    try:
        stats = compute_stats(df)
        print("\n=== Statistiche per sensore ===")
        print(stats)
    except Exception as e:
        print(f"[WARNING] Errore statistiche: {e}")

    # =========================
    # 4. Transform
    # =========================
    try:
        pivot_df = pivot_timeseries(df)
    except Exception as e:
        print(f"[ERROR] Errore pivot: {e}")
        sys.exit(1)

    # =========================
    # 5. Derivative
    # =========================
    try:
        diff_df = compute_derivative(pivot_df)
    except Exception as e:
        print(f"[WARNING] Errore derivata: {e}")
        diff_df = None

    # =========================
    # 6. Plot
    # =========================
    try:
        meta = load_sensor_meta(config_path)
        plot_per_sensor(df, meta = meta)
    except Exception as e:
        print(f"[ERROR] Errore nei plot: {e}")
        sys.exit(1)

    print("[INFO] Analisi completata con successo")


if __name__ == "__main__":
    main()