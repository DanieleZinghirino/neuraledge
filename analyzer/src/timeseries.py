def compute_stats(df):
    return df.groupby("sensor_id")["value"].describe()

def pivot_timeseries(df):
    return df.pivot(index="timestamp", columns="sensor_id", values="value")

def compute_derivative(pivot_df):
    return pivot_df.diff().dropna()