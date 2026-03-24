#!/bin/bash

set -e

# =========================================================
# ROOT DEL REPOSITORY
# =========================================================
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

EDGE_DIR="$REPO_ROOT/edge"
EDGE_BUILD_DIR="$EDGE_DIR/build"

ANALYZER_DIR="$REPO_ROOT/analyzer"
ANALYZER_VENV_DIR="$ANALYZER_DIR/venv"

# =========================================================
# CONFIG DEFAULT
# Se non passi nulla, usa edge/config/simulation.yaml
# Se passi un argomento, usa quello
# =========================================================
if [ $# -ge 1 ]; then
    CONFIG_FILE="$1"
else
    CONFIG_FILE="$EDGE_DIR/config/simulation.yaml"
fi

# Se il path passato è relativo, rendilo assoluto rispetto alla root repo
case "$CONFIG_FILE" in
    /*) ;;
    *) CONFIG_FILE="$REPO_ROOT/$CONFIG_FILE" ;;
esac

echo "===================================="
echo "🚀 NeuralEdge Pipeline Starting..."
echo "===================================="
echo "[INFO] Repository root : $REPO_ROOT"
echo "[INFO] Config file     : $CONFIG_FILE"

# =========================================================
# CHECK CONFIG
# =========================================================
if [ ! -f "$CONFIG_FILE" ]; then
    echo "[ERROR] Config file not found: $CONFIG_FILE"
    exit 1
fi

# =========================================================
# 1. BUILD C++
# =========================================================
echo "[STEP 1] Building Edge (C++)..."

cd "$EDGE_DIR"
mkdir -p build
cd "$EDGE_BUILD_DIR"

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

echo "[OK] Build completed"

# =========================================================
# 2. RUN C++
# =========================================================
echo "[STEP 2] Running sensors simulation..."

./edge "$CONFIG_FILE"

echo "[OK] Simulation completed"

# =========================================================
# 3. SETUP PYTHON
# =========================================================
echo "[STEP 3] Setting up Python environment..."

cd "$ANALYZER_DIR"

if [ ! -d "$ANALYZER_VENV_DIR" ]; then
    echo "[INFO] Creating virtual environment..."
    python3 -m venv "$ANALYZER_VENV_DIR"
fi

"$ANALYZER_VENV_DIR/bin/pip" install -r requirements.txt --quiet

echo "[OK] Python environment ready"

# =========================================================
# 4. RUN PYTHON ANALYZER
# Passiamo lo stesso YAML: il Python ricaverà da lì il CSV
# =========================================================
echo "[STEP 4] Running output analysis..."

"$ANALYZER_VENV_DIR/bin/python" src/main.py "$CONFIG_FILE"

echo "[OK] Analysis completed"

# =========================================================
# DONE
# =========================================================
echo "===================================="
echo "✅ Finished!"
echo "===================================="