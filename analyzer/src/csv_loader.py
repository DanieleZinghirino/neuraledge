import pandas as pd

def load_csv(path: str) -> pd.DataFrame:
    """Load a CSV file into a pandas DataFrame."""
    df = pd.read_csv(path)
    df["timestamp"] = pd.to_datetime(df["timestamp"])
    df = df.sort_values("timestamp")
    return df