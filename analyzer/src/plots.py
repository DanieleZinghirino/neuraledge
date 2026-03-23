from pathlib import Path
import shutil
import matplotlib.pyplot as plt


def _get_output_dir() -> Path:
    """
    BUG FIX:
    - La cartella veniva ricreata solo se esisteva già (logica invertita).
    - La funzione poteva ritornare None implicitamente se il ramo if non entrava.
    - Ora: cancella se esiste, poi crea sempre con parents=True.
    """
    output_dir = Path(__file__).resolve().parents[2] / "output/plots"

    if output_dir.exists():
        shutil.rmtree(output_dir)

    output_dir.mkdir(parents=True, exist_ok=True)

    return output_dir


def plot_per_sensor(df):
    output_dir = _get_output_dir()

    sensors = df["sensor_id"].unique()

    for sensor in sensors:
        sensor_df = df[df["sensor_id"] == sensor].copy()

        # Ordinamento temporale
        sensor_df = sensor_df.sort_values("timestamp")

        # Rolling mean
        sensor_df["rolling"] = sensor_df["value"].rolling(window=5).mean()

        # Statistiche
        mean_value = sensor_df["value"].mean()
        std_value = sensor_df["value"].std()

        # Soglie anomalie
        k = 2  # puoi mettere 3 per più rigore
        upper_threshold = mean_value + k * std_value
        lower_threshold = mean_value - k * std_value

        # Selezione anomalie
        high_anomalies = sensor_df[sensor_df["value"] > upper_threshold]
        low_anomalies = sensor_df[sensor_df["value"] < lower_threshold]

        # Unità di misura
        unit = sensor_df["unit"].iloc[0]

        plt.figure(figsize=(11, 5))

        # Segnale reale
        plt.plot(
            sensor_df["timestamp"],
            sensor_df["value"],
            label="Raw",
            color="green",
            alpha=0.6
        )

        # Trend
        plt.plot(
            sensor_df["timestamp"],
            sensor_df["rolling"],
            linestyle="--",
            color="orange",
            label="Trend"
        )

        # Media
        plt.axhline(
            mean_value,
            linestyle=":",
            color="black",
            label=f"Mean = {mean_value:.2f}"
        )

        # Bande di soglia
        plt.axhline(
            upper_threshold,
            linestyle="--",
            color="red",
            alpha=0.5,
            label="+2σ"
        )

        plt.axhline(
            lower_threshold,
            linestyle="--",
            color="blue",
            alpha=0.5,
            label="-2σ"
        )

        # 🔴 Anomalie alte
        plt.scatter(
            high_anomalies["timestamp"],
            high_anomalies["value"],
            color="red",
            s=60,
            label="High anomaly"
        )

        # 🔵 Anomalie basse
        plt.scatter(
            low_anomalies["timestamp"],
            low_anomalies["value"],
            color="cyan",
            s=60,
            label="Low anomaly"
        )

        # Fill solo per anomalie (NON tutto sopra/sotto)
        plt.fill_between(
            sensor_df["timestamp"],
            sensor_df["value"],
            upper_threshold,
            where=(sensor_df["value"] > upper_threshold),
            color="red",
            alpha=0.2
        )

        plt.fill_between(
            sensor_df["timestamp"],
            sensor_df["value"],
            lower_threshold,
            where=(sensor_df["value"] < lower_threshold),
            color="cyan",
            alpha=0.2
        )

        plt.title(f"Sensor: {sensor}")
        plt.xlabel("Time")
        plt.ylabel(f"Value ({unit})")

        plt.legend()
        plt.grid(True)
        plt.xticks(rotation=45)

        output_file = output_dir / f"{sensor}.png"
        plt.savefig(output_file)

        print(f"[INFO] Salvato: {output_file}")

        plt.close()