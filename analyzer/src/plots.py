from __future__ import annotations

import shutil
from pathlib import Path

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
import pandas as pd

from sensor_meta import SensorMeta


# =========================================================
# PALETTE FAULT — colore del bordo/titolo in base al tipo
# =========================================================
_FAULT_COLOR: dict[str, str] = {
    "none":       "#4CAF50",   # verde  — nessun guasto
    "nofault":    "#4CAF50",
    "spike":      "#FF9800",   # arancio — guasto impulsivo
    "stuck":      "#9C27B0",   # viola  — sensore bloccato
    "drift":      "#F44336",   # rosso  — deriva progressiva
    "scaling":    "#2196F3",   # blu    — errore di scala
    "saturation": "#FF5722",   # arancio scuro — saturazione
    "dropout":    "#607D8B",   # grigio blu — dati mancanti
}

_NOISE_LABEL: dict[str, str] = {
    "gaussian": "Gaussian",
    "uniform":  "Uniform",
    "pink":     "Pink (1/f)",
    "drift":    "Drift (RW)",
    "periodic": "Periodic (50/100 Hz)",
    "impulse":  "Impulse (EMI)",
}

_FAULT_LABEL: dict[str, str] = {
    "none":       "No fault",
    "nofault":    "No fault",
    "spike":      "Spike",
    "stuck":      "Stuck",
    "drift":      "Drift fault",
    "scaling":    "Scaling",
    "saturation": "Saturation",
    "dropout":    "Dropout (NaN)",
}


def _get_output_dir() -> Path:
    output_dir = Path(__file__).resolve().parents[2] / "output/plots"
    if output_dir.exists():
        shutil.rmtree(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    return output_dir


def _fault_color(fault_model: str) -> str:
    return _FAULT_COLOR.get(fault_model.lower(), "#9E9E9E")


def _noise_label(noise_model: str) -> str:
    return _NOISE_LABEL.get(noise_model.lower(), noise_model)


def _fault_label(fault_model: str) -> str:
    return _FAULT_LABEL.get(fault_model.lower(), fault_model)


def plot_per_sensor(
    df: pd.DataFrame,
    meta: dict[str, SensorMeta] | None = None,
) -> None:
    """
    Genera un grafico per ogni sensore con:
    - segnale raw + trend (rolling mean)
    - bande ±2σ e anomalie evidenziate
    - gap visivi per campioni NaN (DropoutFault)
    - titolo con noise model e fault model
    - colore del bordo coerente con il tipo di fault
    - riquadro informativo con statistiche e metadata
    """
    output_dir = _get_output_dir()

    for sensor_id in df["sensor_id"].unique():
        sensor_df = (
            df[df["sensor_id"] == sensor_id]
            .copy()
            .sort_values("timestamp")
            .reset_index(drop=True)
        )

        unit          = sensor_df["unit"].iloc[0]
        smeta         = meta.get(sensor_id) if meta else None
        noise_model   = smeta.noise_model   if smeta else "unknown"
        fault_model   = smeta.fault_model   if smeta else "unknown"
        sensor_type   = smeta.sensor_type   if smeta else "unknown"
        sampling_rate = smeta.sampling_rate if smeta else 0.0

        fc = _fault_color(fault_model)

        # =========================================================
        # STATISTICHE (escludono NaN automaticamente)
        # =========================================================
        mean_val  = sensor_df["value"].mean()
        std_val   = sensor_df["value"].std()
        k         = 2
        upper_thr = mean_val + k * std_val
        lower_thr = mean_val - k * std_val

        total_samples   = len(sensor_df)
        nan_count       = sensor_df["value"].isna().sum()
        nan_pct         = 100.0 * nan_count / total_samples if total_samples > 0 else 0.0

        high_anomalies  = sensor_df[sensor_df["value"] > upper_thr]
        low_anomalies   = sensor_df[sensor_df["value"] < lower_thr]
        anomaly_count   = len(high_anomalies) + len(low_anomalies)

        # Rolling mean (ignora NaN)
        sensor_df["rolling"] = (
            sensor_df["value"]
            .rolling(window=max(3, total_samples // 30), min_periods=1)
            .mean()
        )

        # =========================================================
        # FIGURA
        # =========================================================
        fig, ax = plt.subplots(figsize=(13, 5))
        fig.patch.set_facecolor("#FAFAFA")
        ax.set_facecolor("#F5F5F5")

        # Bordo colorato in base al fault
        for spine in ax.spines.values():
            spine.set_edgecolor(fc)
            spine.set_linewidth(2.5)

        # =========================================================
        # GAP VISIVI PER NaN (DropoutFault)
        # Colora le regioni dove il valore è NaN
        # =========================================================
        if nan_count > 0:
            in_gap   = False
            gap_start = None

            for idx, row in sensor_df.iterrows():
                is_nan = pd.isna(row["value"])
                if is_nan and not in_gap:
                    gap_start = row["timestamp"]
                    in_gap    = True
                elif not is_nan and in_gap:
                    ax.axvspan(gap_start, row["timestamp"],
                               color="#607D8B", alpha=0.25, zorder=1,
                               label="_dropout_region")
                    in_gap = False

            # Gap aperto fino alla fine
            if in_gap and gap_start is not None:
                ax.axvspan(gap_start, sensor_df["timestamp"].iloc[-1],
                           color="#607D8B", alpha=0.25, zorder=1)

        # =========================================================
        # SEGNALE RAW
        # =========================================================
        ax.plot(
            sensor_df["timestamp"],
            sensor_df["value"],
            color="#43A047",
            alpha=0.7,
            linewidth=0.9,
            label="Raw signal",
            zorder=3,
        )

        # =========================================================
        # TREND (rolling mean)
        # =========================================================
        ax.plot(
            sensor_df["timestamp"],
            sensor_df["rolling"],
            linestyle="--",
            color="#FB8C00",
            linewidth=1.5,
            label="Trend (rolling mean)",
            zorder=4,
        )

        # =========================================================
        # BANDE ±kσ
        # =========================================================
        ax.axhline(mean_val,  linestyle=":",  color="#212121", linewidth=1.2,
                   label=f"Mean = {mean_val:.3f}", zorder=2)
        ax.axhline(upper_thr, linestyle="--", color="#E53935", alpha=0.6,
                   linewidth=1.0, label=f"+{k}σ = {upper_thr:.3f}", zorder=2)
        ax.axhline(lower_thr, linestyle="--", color="#1E88E5", alpha=0.6,
                   linewidth=1.0, label=f"−{k}σ = {lower_thr:.3f}", zorder=2)

        # =========================================================
        # FILL ANOMALIE
        # =========================================================
        ax.fill_between(
            sensor_df["timestamp"], sensor_df["value"], upper_thr,
            where=(sensor_df["value"] > upper_thr),
            color="#E53935", alpha=0.15, zorder=2,
        )
        ax.fill_between(
            sensor_df["timestamp"], sensor_df["value"], lower_thr,
            where=(sensor_df["value"] < lower_thr),
            color="#1E88E5", alpha=0.15, zorder=2,
        )

        # =========================================================
        # SCATTER ANOMALIE
        # =========================================================
        if not high_anomalies.empty:
            ax.scatter(
                high_anomalies["timestamp"], high_anomalies["value"],
                color="#E53935", s=40, zorder=5, label="High anomaly",
            )
        if not low_anomalies.empty:
            ax.scatter(
                low_anomalies["timestamp"], low_anomalies["value"],
                color="#1E88E5", s=40, zorder=5, label="Low anomaly",
            )

        # =========================================================
        # TITOLO
        # =========================================================
        title_lines = [
            f"Sensor: {sensor_id}  |  type: {sensor_type}  |  "
            f"rate: {sampling_rate:.0f} Hz",
            f"noise: {_noise_label(noise_model)}   "
            f"fault: {_fault_label(fault_model)}",
        ]
        ax.set_title(
            "\n".join(title_lines),
            fontsize=10,
            loc="left",
            pad=10,
            color="#212121",
        )

        # =========================================================
        # RIQUADRO INFORMATIVO (stats + metadata)
        # =========================================================
        info_lines = [
            f"samples : {total_samples}",
            f"NaN     : {nan_count} ({nan_pct:.1f}%)",
            f"anomalies: {anomaly_count}",
            f"std     : {std_val:.4f}",
            f"min     : {sensor_df['value'].min():.4f}",
            f"max     : {sensor_df['value'].max():.4f}",
        ]
        info_text = "\n".join(info_lines)

        ax.text(
            1.01, 0.98, info_text,
            transform=ax.transAxes,
            fontsize=8,
            verticalalignment="top",
            fontfamily="monospace",
            bbox=dict(
                boxstyle="round,pad=0.5",
                facecolor="white",
                edgecolor=fc,
                linewidth=1.5,
                alpha=0.9,
            ),
        )

        # =========================================================
        # LEGEND + ASSI
        # =========================================================
        ax.set_xlabel("Time", fontsize=9)
        ax.set_ylabel(f"Value ({unit})", fontsize=9)
        ax.legend(loc="upper left", fontsize=7.5, framealpha=0.85)
        ax.grid(True, linestyle="--", alpha=0.4, color="#BDBDBD")
        plt.xticks(rotation=35, fontsize=7.5)
        plt.yticks(fontsize=8)

        plt.tight_layout()

        output_file = output_dir / f"{sensor_id}.png"
        plt.savefig(output_file, dpi=130, bbox_inches="tight")
        plt.close()

        print(f"[INFO] Salvato: {output_file}")