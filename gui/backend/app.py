from pathlib import Path
import subprocess
import threading

import yaml
from flask import Flask, jsonify, request
from flask_cors import CORS

from model_schema import SIGNAL_MODELS, NOISE_MODELS, FAULT_MODELS

app = Flask(__name__)
CORS(app)

REPO_ROOT = Path(__file__).resolve().parents[2]
CONFIG_DIR = REPO_ROOT / "edge" / "config"
RUN_SCRIPT = REPO_ROOT / "run.sh"

run_state = {
    "status": "idle",   # idle | running | completed | failed
    "logs": "",
    "return_code": None,
    "config": None,
}


def list_yaml_files():
    CONFIG_DIR.mkdir(parents=True, exist_ok=True)
    return sorted([p.name for p in CONFIG_DIR.glob("*.yaml")])


def validate_filename(filename: str):
    if not filename.endswith(".yaml"):
        raise ValueError("Filename must end with .yaml")
    if "/" in filename or "\\" in filename:
        raise ValueError("Filename must not contain path separators")
    if filename.strip() == "":
        raise ValueError("Filename cannot be empty")


def load_yaml_file(filename: str):
    validate_filename(filename)

    path = CONFIG_DIR / filename
    if not path.exists():
        raise FileNotFoundError(f"Config file not found: {filename}")

    with open(path, "r", encoding="utf-8") as f:
        data = yaml.safe_load(f)

    if data is None:
        data = {}

    return data


def save_yaml_file(filename: str, data):
    validate_filename(filename)

    path = CONFIG_DIR / filename
    path.parent.mkdir(parents=True, exist_ok=True)

    with open(path, "w", encoding="utf-8") as f:
        yaml.safe_dump(data, f, sort_keys=False, allow_unicode=True)


def build_default_config():
    return {
        "simulation": {
            "seed": 42,
            "duration_seconds": 30,
            "output_csv": "../../output/sensor_data.csv",
        },
        "sensors": [],
    }


def run_pipeline_in_background(config_filename: str):
    global run_state

    config_relative = f"edge/config/{config_filename}"

    run_state["status"] = "running"
    run_state["logs"] = ""
    run_state["return_code"] = None
    run_state["config"] = config_filename

    try:
        process = subprocess.Popen(
            [str(RUN_SCRIPT), config_relative],
            cwd=str(REPO_ROOT),
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,
        )

        collected_logs = []

        assert process.stdout is not None
        for line in process.stdout:
            collected_logs.append(line)

        process.wait()

        run_state["logs"] = "".join(collected_logs)
        run_state["return_code"] = process.returncode
        run_state["status"] = "completed" if process.returncode == 0 else "failed"

    except Exception as e:
        run_state["logs"] += f"\n[BACKEND ERROR] {e}\n"
        run_state["return_code"] = -1
        run_state["status"] = "failed"


@app.route("/api/schema", methods=["GET"])
def get_schema():
    return jsonify({
        "signals": SIGNAL_MODELS,
        "noises": NOISE_MODELS,
        "faults": FAULT_MODELS,
    })


@app.route("/api/configs", methods=["GET"])
def get_configs():
    return jsonify(list_yaml_files())


@app.route("/api/configs/<filename>", methods=["GET"])
def get_config(filename):
    try:
        return jsonify(load_yaml_file(filename))
    except Exception as e:
        return jsonify({"error": str(e)}), 404


@app.route("/api/configs/<filename>", methods=["PUT"])
def put_config(filename):
    try:
        data = request.get_json()
        if data is None:
            return jsonify({"error": "Missing JSON body"}), 400

        save_yaml_file(filename, data)
        return jsonify({"message": f"{filename} saved successfully"})
    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route("/api/configs/save-as", methods=["POST"])
def save_as_config():
    try:
        data = request.get_json()
        if data is None:
            return jsonify({"error": "Missing JSON body"}), 400

        filename = data.get("filename")
        config = data.get("config")

        if filename is None or config is None:
            return jsonify({"error": "Both 'filename' and 'config' are required"}), 400

        validate_filename(filename)

        save_yaml_file(filename, config)
        return jsonify({
            "message": f"{filename} saved successfully",
            "filename": filename
        })
    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route("/api/configs/new", methods=["GET"])
def new_config():
    return jsonify(build_default_config())


@app.route("/api/run", methods=["POST"])
def run_config():
    global run_state

    if run_state["status"] == "running":
        return jsonify({"error": "A simulation is already running"}), 409

    data = request.get_json()
    if data is None or "config" not in data:
        return jsonify({"error": "Missing 'config' field"}), 400

    filename = data["config"]
    path = CONFIG_DIR / filename
    if not path.exists():
        return jsonify({"error": f"Config file not found: {filename}"}), 404

    thread = threading.Thread(
        target=run_pipeline_in_background,
        args=(filename,),
        daemon=True,
    )
    thread.start()

    return jsonify({"message": f"Simulation started with {filename}"})


@app.route("/api/run/status", methods=["GET"])
def get_run_status():
    return jsonify(run_state)


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=True)