from pathlib import Path
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


def _get_output_dir() -> Path:
    output_dir = Path(__file__).resolve().parents[2] / "output"
    output_dir.mkdir(exist_ok=True)
    return output_dir


# =========================
# 1. TIME SERIES + ANOMALIE
# =========================
def plot_timeseries_with_anomalies(pivot_df):
    output_dir = _get_output_dir()

    plt.figure(figsize=(14, 7))

    for col in pivot_df.columns:
        series = pivot_df[col]

        # Z-score per anomalie
        z = (series - series.mean()) / series.std()
        anomalies = series[np.abs(z) > 3]

        plt.plot(series.index, series, label=col)
        plt.scatter(anomalies.index, anomalies, marker="x", s=80)

    plt.title("Time Series + Anomalies (Z-score > 3)")
    plt.legend()
    plt.grid(True)

    out = output_dir / "timeseries_anomalies.png"
    plt.savefig(out)
    print(f"[INFO] {out}")

    plt.close()


# =========================
# 2. DISTRIBUZIONE (BOXPLOT)
# =========================
def plot_distributions(pivot_df):
    output_dir = _get_output_dir()

    plt.figure(figsize=(10, 6))
    pivot_df.boxplot()

    plt.title("Distribuzione per sensore (Boxplot)")
    plt.grid(True)

    out = output_dir / "distributions_boxplot.png"
    plt.savefig(out)
    print(f"[INFO] {out}")

    plt.close()


# =========================
# 3. CORRELAZIONE
# =========================
def plot_correlation(pivot_df):
    output_dir = _get_output_dir()

    corr = pivot_df.corr()

    plt.figure(figsize=(8, 6))
    plt.imshow(corr)
    plt.colorbar()

    plt.xticks(range(len(corr)), corr.columns, rotation=45)
    plt.yticks(range(len(corr)), corr.columns)

    plt.title("Correlation Matrix")

    out = output_dir / "correlation.png"
    plt.savefig(out)
    print(f"[INFO] {out}")

    plt.close()


# =========================
# 4. DERIVATA (DINAMICA)
# =========================
def plot_derivative(diff_df):
    output_dir = _get_output_dir()

    plt.figure(figsize=(14, 6))
    diff_df.plot()

    plt.title("Rate of Change (Derivative)")
    plt.grid(True)

    out = output_dir / "derivative.png"
    plt.savefig(out)
    print(f"[INFO] {out}")

    plt.close()


# =========================
# 5. HEATMAP TEMPORALE
# =========================
def plot_time_heatmap(pivot_df):
    output_dir = _get_output_dir()

    # normalizzazione
    norm = (pivot_df - pivot_df.min()) / (pivot_df.max() - pivot_df.min())

    plt.figure(figsize=(14, 6))
    plt.imshow(norm.T, aspect='auto')

    plt.yticks(range(len(norm.columns)), norm.columns)
    plt.title("Temporal Heatmap (Normalized)")

    plt.colorbar()

    out = output_dir / "heatmap.png"
    plt.savefig(out)
    print(f"[INFO] {out}")

    plt.close()