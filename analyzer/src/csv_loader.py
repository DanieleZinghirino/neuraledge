import pandas as pd
from pathlib import Path


def load_csv(path: str | Path) -> pd.DataFrame:
    """
    Carica il CSV prodotto dal simulatore C++ in un DataFrame pandas.

    GESTIONE NaN:
    I campioni mancanti (DropoutFault) vengono scritti nel CSV con
    la cella value vuota. pandas li interpreta automaticamente come
    NaN grazie a na_values, che copre anche le stringhe "nan" e "NaN"
    nel caso di scritture esplicite.

    COLONNE ATTESE:
        timestamp  — datetime con millisecondi (es. 2026-03-19 14:32:01.123)
        sensor_id  — identificativo stringa    (es. temp1, curr2, speed1)
        value      — float64, può contenere NaN
        unit       — stringa unità di misura   (es. C, A, m/s)

    Returns:
        DataFrame ordinato per timestamp con tipi corretti.

    Raises:
        FileNotFoundError: se il file non esiste.
        ValueError:        se le colonne attese sono assenti.
    """
    path = Path(path)

    if not path.exists():
        raise FileNotFoundError(f"CSV non trovato: {path}")

    df = pd.read_csv(
        path,
        na_values=["", "nan", "NaN", "NAN"],  # celle vuote e varianti stringa → NaN
        keep_default_na=True,
    )

    # =========================================================
    # VALIDAZIONE COLONNE
    # Fallisce esplicitamente se il CSV non ha la struttura attesa,
    # invece di propagare KeyError criptici nell'analyzer.
    # =========================================================
    expected_columns = {"timestamp", "sensor_id", "value", "unit"}
    missing = expected_columns - set(df.columns)
    if missing:
        raise ValueError(
            f"Colonne mancanti nel CSV: {missing}. "
            f"Colonne trovate: {list(df.columns)}"
        )

    # =========================================================
    # TIPO TIMESTAMP
    # errors="coerce" trasforma timestamp malformati in NaT
    # invece di lanciare eccezioni, rendendo il caricamento robusto
    # a righe corrotte o parzialmente scritte durante un crash.
    # =========================================================
    df["timestamp"] = pd.to_datetime(df["timestamp"], errors="coerce")

    # Rimuove righe con timestamp non parsabile (NaT) — dati irrecuperabili
    invalid_ts = df["timestamp"].isna().sum()
    if invalid_ts > 0:
        print(f"[WARNING] Rimosse {invalid_ts} righe con timestamp non valido")
        df = df.dropna(subset=["timestamp"])

    # =========================================================
    # TIPO VALUE
    # Forza la conversione a float64 — gestisce eventuali stringhe
    # residue o valori non numerici con errors="coerce" → NaN.
    # =========================================================
    df["value"] = pd.to_numeric(df["value"], errors="coerce")

    # Ordinamento temporale — essenziale per rolling, plot e derivate
    df = df.sort_values("timestamp").reset_index(drop=True)

    # =========================================================
    # REPORT QUALITÀ DATI
    # Stampa un riepilogo dei NaN per sensore — utile per diagnosticare
    # DropoutFault e validare la simulazione.
    # =========================================================
    nan_counts = df.groupby("sensor_id")["value"].apply(lambda x: x.isna().sum())
    nan_counts = nan_counts[nan_counts > 0]
    if not nan_counts.empty:
        print("[INFO] Campioni mancanti (NaN) per sensore:")
        for sensor_id, count in nan_counts.items():
            total = len(df[df["sensor_id"] == sensor_id])
            print(f"       {sensor_id}: {count}/{total} "
                  f"({100 * count / total:.1f}%)")

    return df