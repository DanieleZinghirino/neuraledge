#!/bin/bash

set -e  # interrompe se qualcosa fallisce

echo "===================================="
echo "🚀 NeuralEdge Pipeline Starting..."
echo "===================================="

# =========================
# 1. BUILD C++
# =========================
echo "[STEP 1] Building Edge (C++)..."

cd edge
mkdir -p build
cd build

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

echo "[OK] Build completata"

# =========================
# 2. RUN C++
# =========================
echo "[STEP 2] Running sensors simulation..."

./edge

echo "[OK] Simulation completed"

# =========================
# 3. SETUP PYTHON
# =========================
echo "[STEP 3] Setting up Python environment..."

cd ../../analyzer

if [ ! -d "venv" ]; then
    echo "[INFO] Creating virtual environment..."
    python3 -m venv venv
fi

venv/bin/pip install -r requirements.txt --quiet

echo "[OK] Python environment ready"

# =========================
# 4. RUN PYTHON
# =========================
echo "[STEP 4] Running output analysis..."

venv/bin/python src/main.py

echo "[OK] Analysis completed"

# =========================
# DONE
# =========================
echo "===================================="
echo "✅ Finished!"
echo "Plots in: output/plots"
echo "===================================="