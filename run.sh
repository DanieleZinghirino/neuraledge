#!/bin/bash

set -e  # interrompe se qualcosa fallisce

echo "===================================="
echo "🚀 NeuralEdge Pipeline Starting..."
echo "===================================="

# =========================
# 1. BUILD C++
# =========================
echo "[STEP 1] Building Edge (C++)..."

cd edge || exit
mkdir -p build
cd build || exit

cmake ..
cmake --build .

echo "[OK] Build completata"

# =========================
# 2. RUN C++
# =========================
echo "[STEP 2] Running sensors simulation..."

./edge

echo "[OK] Simulation completed"

# =========================
# 3. RUN PYTHON
# =========================
echo "[STEP 3] Running output analysis..."

cd ../../analyzer|| exit

# Usa direttamente il Python del venv
venv/bin/python src/main.py

echo "[OK] Analysis completed"

# =========================
# DONE
# =========================
echo "===================================="
echo "✅ Finished!"
echo "Plots in: output/plots"
echo "===================================="