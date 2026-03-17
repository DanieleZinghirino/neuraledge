from pathlib import Path
import sys

from csv_loader import load_csv
from timeseries import pivot_timeseries, compute_derivative, compute_stats
from plots import (
    plot_timeseries_with_anomalies,
    plot_distributions,
    plot_correlation,
    plot_derivative,
    plot_time_heatmap
)


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


def resolve_csv_path(workspace_root: Path) -> Path:
    """
    Path al CSV condiviso con C++
    """
    csv_path = workspace_root / "output/sensor_data.csv"

    if not csv_path.exists():
        raise FileNotFoundError(
            f"CSV non trovato in: {csv_path}\n"
            "Assicurati che il programma C++ abbia generato il file."
        )

    return csv_path


def main():
    print("=== NeuralEdge Analyzer ===")

    # =========================
    # 1. Path resolution
    # =========================
    workspace_root = resolve_workspace_root()
    csv_path = resolve_csv_path(workspace_root)

    print(f"[INFO] Workspace: {workspace_root}")
    print(f"[INFO] CSV path: {csv_path}")

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
        plot_timeseries_with_anomalies(pivot_df)
        plot_distributions(pivot_df)
        plot_correlation(pivot_df)
        plot_derivative(diff_df)
        plot_time_heatmap(pivot_df)

    except Exception as e:
        print(f"[ERROR] Errore nei plot: {e}")
        sys.exit(1)

    print("[INFO] Analisi completata con successo")


if __name__ == "__main__":
    main()