def compute_stats(df):
    return df.groupby("sensor_id")["value"].describe()

def pivot_timeseries(df):
    # Più thread possono produrre lo stesso timestamp al millisecondo.
    # Raggruppiamo per (timestamp, sensor_id) facendo la media dei duplicati
    # prima del pivot — in pratica non cambia nulla sui dati reali perché
    # i duplicati esatti sono rarissimi, ma rende il pivot robusto.
    df_deduped = (
        df.groupby(["timestamp", "sensor_id"])["value"]
        .mean()
        .reset_index()
    )
    return df_deduped.pivot(index="timestamp", columns="sensor_id", values="value")

def compute_derivative(pivot_df):
    return pivot_df.diff().dropna()