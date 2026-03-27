SIGNAL_MODELS = {
    "temperature": {
        "label": "Temperature",
        "params": [
            {"name": "base", "type": "number", "default": 25.0},
            {"name": "tau", "type": "number", "default": 100.0},
            {"name": "slow_amp", "type": "number", "default": 1.0},
            {"name": "slow_freq", "type": "number", "default": 0.001},
            {"name": "fast_amp", "type": "number", "default": 0.05},
            {"name": "fast_freq", "type": "number", "default": 0.03},
        ],
    },
    "current": {
        "label": "Current",
        "params": [
            {"name": "base", "type": "number", "default": 1.0},
            {"name": "fundamental", "type": "number", "default": 2.5},
            {"name": "freq", "type": "number", "default": 50.0},
            {"name": "thd3_amp", "type": "number", "default": 0.15},
            {"name": "thd3_phase", "type": "number", "default": 0.1},
            {"name": "thd5_amp", "type": "number", "default": 0.05},
            {"name": "thd5_phase", "type": "number", "default": 0.2},
            {"name": "load_period", "type": "number", "default": 10.0},
            {"name": "delta_i", "type": "number", "default": 1.0},
            {"name": "tau_load", "type": "number", "default": 1.0},
        ],
    },
    "speed": {
        "label": "Speed",
        "params": [
            {"name": "vmax", "type": "number", "default": 2.0},
            {"name": "t_acc", "type": "number", "default": 2.0},
            {"name": "t_cruise", "type": "number", "default": 8.0},
            {"name": "t_dec", "type": "number", "default": 2.0},
            {"name": "t_stop", "type": "number", "default": 3.0},
        ],
    },
    "vibration": {
        "label": "Vibration",
        "params": [
            {"name": "base_freq", "type": "number", "default": 25.0},
            {"name": "amp1", "type": "number", "default": 0.5},
            {"name": "amp2", "type": "number", "default": 0.15},
            {"name": "amp3", "type": "number", "default": 0.06},
            {"name": "phase1", "type": "number", "default": 0.0},
            {"name": "phase2", "type": "number", "default": 0.2},
            {"name": "phase3", "type": "number", "default": 0.4},
        ],
    },
    "pressure": {
        "label": "Pressure",
        "params": [
            {"name": "baseline", "type": "number", "default": 3.0},
            {"name": "slow_amplitude", "type": "number", "default": 0.1},
            {"name": "slow_freq", "type": "number", "default": 0.002},
            {"name": "transient_prob", "type": "number", "default": 0.003},
            {"name": "transient_mag", "type": "number", "default": 0.4},
            {"name": "transient_tau", "type": "number", "default": 1.5},
            {"name": "p_max", "type": "number", "default": 6.0},
        ],
    },
    "humidity": {
        "label": "Humidity",
        "params": [
            {"name": "base", "type": "number", "default": 45.0},
            {"name": "amplitude", "type": "number", "default": 5.0},
            {"name": "frequency", "type": "number", "default": 0.001},
        ],
    },
    "square_wave": {
        "label": "Square Wave",
        "params": [
            {"name": "period", "type": "number", "default": 10.0},
            {"name": "duty_cycle", "type": "number", "default": 0.5},
        ],
    },
    "ramp": {
        "label": "Ramp",
        "params": [
            {"name": "period", "type": "number", "default": 20.0},
            {"name": "low", "type": "number", "default": 0.0},
            {"name": "high", "type": "number", "default": 40.0},
        ],
    },
    "step_response": {
        "label": "Step Response",
        "params": [
            {"name": "high", "type": "number", "default": 60.0},
            {"name": "low", "type": "number", "default": 20.0},
            {"name": "tau", "type": "number", "default": 3.0},
            {"name": "period", "type": "number", "default": 20.0},
        ],
    },
}

NOISE_MODELS = {
    "gaussian": {
        "label": "Gaussian Noise",
        "params": [{"name": "sigma", "type": "number", "default": 0.05}],
    },
    "uniform": {
        "label": "Uniform Noise",
        "params": [{"name": "amplitude", "type": "number", "default": 0.02}],
    },
    "pink": {
        "label": "Pink Noise",
        "params": [{"name": "sigma", "type": "number", "default": 0.03}],
    },
    "drift": {
        "label": "Drift Noise",
        "params": [
            {"name": "step_sigma", "type": "number", "default": 0.001},
            {"name": "max_drift", "type": "number", "default": 0.1},
        ],
    },
    "periodic": {
        "label": "Periodic Noise",
        "params": [
            {"name": "amplitude", "type": "number", "default": 0.03},
            {"name": "frequency", "type": "number", "default": 50.0},
            {"name": "phase", "type": "number", "default": 0.0},
        ],
    },
    "impulse": {
        "label": "Impulse Noise",
        "params": [
            {"name": "probability", "type": "number", "default": 0.005},
            {"name": "impulse_sigma", "type": "number", "default": 0.08},
        ],
    },
}

FAULT_MODELS = {
    "none": {
        "label": "No Fault",
        "params": [],
    },
    "spike": {
        "label": "Spike Fault",
        "params": [
            {"name": "probability", "type": "number", "default": 0.01},
            {"name": "magnitude", "type": "number", "default": 1.0},
        ],
    },
    "stuck": {
        "label": "Stuck Fault",
        "params": [{"name": "stuck_prob", "type": "number", "default": 0.003}],
    },
    "drift": {
        "label": "Drift Fault",
        "params": [
            {"name": "activation_prob", "type": "number", "default": 0.003},
            {"name": "drift_rate", "type": "number", "default": 0.01},
            {"name": "max_drift", "type": "number", "default": 0.8},
            {"name": "exponent", "type": "number", "default": 1.4},
        ],
    },
    "scaling": {
        "label": "Scaling Fault",
        "params": [
            {"name": "scale_factor", "type": "number", "default": 1.02},
            {"name": "activation_prob", "type": "number", "default": 0.004},
        ],
    },
    "saturation": {
        "label": "Saturation Fault",
        "params": [
            {"name": "lower_bound", "type": "number", "default": 0.0},
            {"name": "upper_bound", "type": "number", "default": 100.0},
            {"name": "activation_prob", "type": "number", "default": 0.004},
        ],
    },
    "dropout": {
        "label": "Dropout Fault",
        "params": [
            {"name": "probability", "type": "number", "default": 0.02},
            {"name": "mode", "type": "text", "default": "random"},
            {"name": "burst_length", "type": "number", "default": 4},
        ],
    },
}