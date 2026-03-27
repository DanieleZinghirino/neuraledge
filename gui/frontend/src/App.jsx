import { useEffect, useState } from "react";
import loadingGif from "./assets/loading.gif";

const API_BASE = "http://127.0.0.1:5000/api";

function ParamInputs({ modelName, modelSchema, params, onChange }) {
  if (!modelName || !modelSchema[modelName]) {
    return null;
  }

  const model = modelSchema[modelName];

  return (
    <div className="param-block">
      {model.params.map((param) => (
        <div key={param.name} className="field">
          <label>{param.name}</label>
          <input
            type={param.type === "number" ? "number" : "text"}
            step={param.type === "number" ? "any" : undefined}
            value={params?.[param.name] ?? param.default}
            onChange={(e) => {
              const value =
                param.type === "number"
                  ? Number(e.target.value)
                  : e.target.value;
              onChange(param.name, value);
            }}
          />
        </div>
      ))}
    </div>
  );
}

function SensorEditor({
  sensor,
  index,
  schema,
  onUpdate,
  onDelete,
  onDuplicate,
}) {
  const signalModels = schema.signals || {};
  const noiseModels = schema.noises || {};
  const faultModels = schema.faults || {};

  const updateField = (field, value) => {
    onUpdate(index, { ...sensor, [field]: value });
  };

  const buildDefaults = (modelMap, key) => {
    const params = {};
    if (modelMap[key]) {
      modelMap[key].params.forEach((p) => {
        params[p.name] = p.default;
      });
    }
    return { model: key, params };
  };

  const updateSensorType = (selectedType) => {
    onUpdate(index, {
      ...sensor,
      type: selectedType,
      signal: buildDefaults(signalModels, selectedType),
    });
  };

  const updateModelName = (section, modelName, modelSchema) => {
    const defaults = {};
    modelSchema[modelName].params.forEach((p) => {
      defaults[p.name] = p.default;
    });

    onUpdate(index, {
      ...sensor,
      [section]: {
        model: modelName,
        params: defaults,
      },
    });
  };

  const updateModelParam = (section, name, value) => {
    onUpdate(index, {
      ...sensor,
      [section]: {
        ...sensor[section],
        params: {
          ...sensor[section].params,
          [name]: value,
        },
      },
    });
  };

  return (
    <div className="sensor-card">
      <div className="sensor-header">
        <h3>Sensore #{index + 1}</h3>
        <div className="sensor-header-actions">
          <button onClick={() => onDuplicate(index)}>Duplica</button>
          <button onClick={() => onDelete(index)}>Rimuovi</button>
        </div>
      </div>

      <div className="field">
        <label>ID</label>
        <input
          value={sensor.id}
          onChange={(e) => updateField("id", e.target.value)}
        />
      </div>

      <div className="field">
        <label>Tipo sensore</label>
        <select
          value={sensor.type}
          onChange={(e) => updateSensorType(e.target.value)}
        >
          <option value="">-- seleziona --</option>
          {Object.entries(signalModels).map(([key, value]) => (
            <option key={key} value={key}>
              {value.label}
            </option>
          ))}
        </select>
      </div>

      <div className="field">
        <label>Sampling rate</label>
        <input
          type="number"
          step="any"
          value={sensor.sampling_rate}
          onChange={(e) => updateField("sampling_rate", Number(e.target.value))}
        />
      </div>

      <h4>Signal</h4>
      <p className="auto-field">
        Il segnale è impostato automaticamente su:{" "}
        <strong>{sensor.signal.model || "non selezionato"}</strong>
      </p>

      <ParamInputs
        modelName={sensor.signal.model}
        modelSchema={signalModels}
        params={sensor.signal.params}
        onChange={(name, value) => updateModelParam("signal", name, value)}
      />

      <h4>Noise</h4>
      <div className="field">
        <label>Noise model</label>
        <select
          value={sensor.noise.model}
          onChange={(e) =>
            updateModelName("noise", e.target.value, noiseModels)
          }
        >
          {Object.entries(noiseModels).map(([key, value]) => (
            <option key={key} value={key}>
              {value.label}
            </option>
          ))}
        </select>
      </div>

      <ParamInputs
        modelName={sensor.noise.model}
        modelSchema={noiseModels}
        params={sensor.noise.params}
        onChange={(name, value) => updateModelParam("noise", name, value)}
      />

      <h4>Fault</h4>
      <div className="field">
        <label>Fault model</label>
        <select
          value={sensor.fault.model}
          onChange={(e) =>
            updateModelName("fault", e.target.value, faultModels)
          }
        >
          {Object.entries(faultModels).map(([key, value]) => (
            <option key={key} value={key}>
              {value.label}
            </option>
          ))}
        </select>
      </div>

      <ParamInputs
        modelName={sensor.fault.model}
        modelSchema={faultModels}
        params={sensor.fault.params}
        onChange={(name, value) => updateModelParam("fault", name, value)}
      />
    </div>
  );
}

function LoadingOverlay({ progress }) {
  return (
    <div className="loading-overlay">
      <div className="loading-card">
        <h2>Simulazione in esecuzione...</h2>

        <img
          src={loadingGif}
          alt="Loading..."
          className="loading-gif"
        />

        <div className="progress-bar">
          <div
            className="progress-bar-fill"
            style={{ width: `${progress}%` }}
          />
        </div>

        <p>{progress}%</p>

        <p className="loading-subtext">
          Simulazione in corso...
        </p>
      </div>
    </div>
  );
}

function PlotModal({ imageUrl, title, zoom, onZoomIn, onZoomOut, onClose }) {
  if (!imageUrl) return null;

  return (
    <div className="modal-overlay" onClick={onClose}>
      <div className="modal-content large" onClick={(e) => e.stopPropagation()}>
        <div className="modal-header">
          <h2>{title}</h2>
          <div className="modal-actions">
            <button onClick={onZoomOut}>-</button>
            <button onClick={onZoomIn}>+</button>
            <button onClick={onClose}>Chiudi</button>
          </div>
        </div>

        <div className="modal-image-container">
          <img
            src={imageUrl}
            alt={title}
            className="modal-image"
            style={{ transform: `scale(${zoom})` }}
          />
        </div>
      </div>
    </div>
  );
}

export default function App() {
  const [schema, setSchema] = useState({ signals: {}, noises: {}, faults: {} });
  const [configFiles, setConfigFiles] = useState([]);
  const [selectedFile, setSelectedFile] = useState("simulation.yaml");
  const [config, setConfig] = useState(null);
  const [runStatus, setRunStatus] = useState(null);
  const [saveAsFilename, setSaveAsFilename] = useState("");
  const [isRunning, setIsRunning] = useState(false);
  const [fakeProgress, setFakeProgress] = useState(0);
  const [plots, setPlots] = useState([]);
  const [selectedPlot, setSelectedPlot] = useState(null);
  const [plotZoom, setPlotZoom] = useState(1);

  useEffect(() => {
    refreshSchemaAndConfigs();
  }, []);

  const refreshSchemaAndConfigs = async () => {
    const schemaRes = await fetch(`${API_BASE}/schema`);
    const schemaData = await schemaRes.json();
    setSchema(schemaData);

    const configsRes = await fetch(`${API_BASE}/configs`);
    const configsData = await configsRes.json();
    setConfigFiles(configsData);
  };

  const fetchPlots = async () => {
    const res = await fetch(`${API_BASE}/plots`);
    const data = await res.json();
    setPlots(data);
  };

  const loadConfig = async () => {
    const res = await fetch(`${API_BASE}/configs/${selectedFile}`);
    const data = await res.json();

    if (data.error) {
      alert(data.error);
      return;
    }

    setConfig(data);
  };

  const createNewConfig = async () => {
    const res = await fetch(`${API_BASE}/configs/new`);
    const data = await res.json();
    setConfig(data);
  };

  const saveConfig = async () => {
    if (!config) return;

    const res = await fetch(`${API_BASE}/configs/${selectedFile}`, {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(config),
    });

    const data = await res.json();
    alert(data.message || data.error);
    await refreshSchemaAndConfigs();
  };

  const saveAsConfig = async () => {
    if (!config) return;
    if (!saveAsFilename.trim()) {
      alert("Inserisci un nome file per Save As");
      return;
    }

    const filename = saveAsFilename.endsWith(".yaml")
      ? saveAsFilename
      : `${saveAsFilename}.yaml`;

    const res = await fetch(`${API_BASE}/configs/save-as`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ filename, config }),
    });

    const data = await res.json();
    if (data.error) {
      alert(data.error);
      return;
    }

    alert(data.message);
    setSelectedFile(filename);
    setSaveAsFilename("");
    await refreshSchemaAndConfigs();
  };

  const runSimulation = async () => {
    setPlots([]);
    setRunStatus(null);
    setIsRunning(true);
    setFakeProgress(5);

    const res = await fetch(`${API_BASE}/run`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ config: selectedFile }),
    });

    const data = await res.json();

    if (data.error) {
      alert(data.error);
      setIsRunning(false);
      setFakeProgress(0);
      return;
    }

    const progressInterval = setInterval(() => {
      setFakeProgress((prev) => {
        if (prev >= 90) return 90;
        return prev + 5;
      });
    }, 700);

    const statusInterval = setInterval(async () => {
      const statusRes = await fetch(`${API_BASE}/run/status`);
      const statusData = await statusRes.json();
      setRunStatus(statusData);

      if (statusData.status !== "running") {
        clearInterval(statusInterval);
        clearInterval(progressInterval);
        setFakeProgress(100);
        setTimeout(() => {
          setIsRunning(false);
          setFakeProgress(0);
        }, 500);

        if (statusData.status === "completed") {
          await fetchPlots();
        }
      }
    }, 1000);
  };

  const buildDefaults = (modelMap, key) => {
    const params = {};
    if (modelMap[key]) {
      modelMap[key].params.forEach((p) => {
        params[p.name] = p.default;
      });
    }
    return { model: key, params };
  };

  const addSensor = () => {
    const firstSignalKey = Object.keys(schema.signals)[0] || "";
    const firstNoiseKey = Object.keys(schema.noises)[0] || "gaussian";
    const firstFaultKey = Object.keys(schema.faults)[0] || "none";

    const newSensor = {
      id: `sensor_${config.sensors.length + 1}`,
      type: firstSignalKey,
      sampling_rate: 1.0,
      signal: buildDefaults(schema.signals, firstSignalKey),
      noise: buildDefaults(schema.noises, firstNoiseKey),
      fault: buildDefaults(schema.faults, firstFaultKey),
    };

    setConfig({
      ...config,
      sensors: [...config.sensors, newSensor],
    });
  };

  const updateSensor = (index, updatedSensor) => {
    const nextSensors = [...config.sensors];
    nextSensors[index] = updatedSensor;
    setConfig({ ...config, sensors: nextSensors });
  };

  const deleteSensor = (index) => {
    const nextSensors = [...config.sensors];
    nextSensors.splice(index, 1);
    setConfig({ ...config, sensors: nextSensors });
  };

  const duplicateSensor = (index) => {
    const source = config.sensors[index];
    const duplicated = structuredClone(source);
    duplicated.id = `${source.id}_copy`;

    const nextSensors = [...config.sensors];
    nextSensors.splice(index + 1, 0, duplicated);

    setConfig({ ...config, sensors: nextSensors });
  };

  const openPlot = (plotName) => {
    setSelectedPlot(plotName);
    setPlotZoom(1);
  };

  const closePlot = () => {
    setSelectedPlot(null);
    setPlotZoom(1);
  };

  return (
    <div className="app">
      <h1>NeuralEdge Config GUI</h1>

      <div className="toolbar">
        <select
          value={selectedFile}
          onChange={(e) => setSelectedFile(e.target.value)}
        >
          {configFiles.map((file) => (
            <option key={file} value={file}>
              {file}
            </option>
          ))}
        </select>

        <button onClick={loadConfig}>Carica YAML</button>
        <button onClick={createNewConfig}>Nuova Config</button>
      </div>

      {config && (
        <>
          <div className="config-box">
            <h2>Simulation</h2>

            <div className="field">
              <label>Seed</label>
              <input
                type="number"
                value={config.simulation.seed}
                onChange={(e) =>
                  setConfig({
                    ...config,
                    simulation: {
                      ...config.simulation,
                      seed: Number(e.target.value),
                    },
                  })
                }
              />
            </div>

            <div className="field">
              <label>Duration (s)</label>
              <input
                type="number"
                step="any"
                value={config.simulation.duration_seconds}
                onChange={(e) =>
                  setConfig({
                    ...config,
                    simulation: {
                      ...config.simulation,
                      duration_seconds: Number(e.target.value),
                    },
                  })
                }
              />
            </div>

            <div className="field">
              <label>Output CSV</label>
              <input
                value={config.simulation.output_csv}
                onChange={(e) =>
                  setConfig({
                    ...config,
                    simulation: {
                      ...config.simulation,
                      output_csv: e.target.value,
                    },
                  })
                }
              />
            </div>
          </div>

          <div className="actions">
            <button onClick={addSensor}>Aggiungi sensore</button>
            <button onClick={saveConfig}>Salva YAML</button>
            <button onClick={runSimulation}>Run Simulation</button>
          </div>

          <div className="sensor-list">
            {config.sensors.map((sensor, index) => (
              <SensorEditor
                key={index}
                sensor={sensor}
                index={index}
                schema={schema}
                onUpdate={updateSensor}
                onDelete={deleteSensor}
                onDuplicate={duplicateSensor}
              />
            ))}
          </div>

          <div className="save-as-box">
            <h2>Save As</h2>
            <div className="save-as-row">
              <input
                placeholder="es. prova_gui.yaml"
                value={saveAsFilename}
                onChange={(e) => setSaveAsFilename(e.target.value)}
              />
              <button onClick={saveAsConfig}>Save As</button>
            </div>
          </div>
        </>
      )}

      {runStatus && !isRunning && (
        <div className="run-status">
          <h2>Run Status</h2>
          <p><strong>Status:</strong> {runStatus.status}</p>
          <p><strong>Config:</strong> {runStatus.config}</p>
          <p><strong>Return code:</strong> {String(runStatus.return_code)}</p>
          <pre>{runStatus.logs}</pre>
        </div>
      )}

      {plots.length > 0 && (
        <div className="plots-section">
          <h2>Grafici generati</h2>
          <div className="plots-grid">
            {plots.map((plot) => (
              <div key={plot} className="plot-card" onClick={() => openPlot(plot)}>
                <img
                  src={`${API_BASE}/plots/${plot}`}
                  alt={plot}
                  className="plot-thumb"
                />
                <p>{plot}</p>
              </div>
            ))}
          </div>
        </div>
      )}

      {isRunning && <LoadingOverlay progress={fakeProgress} />}

      <PlotModal
        imageUrl={selectedPlot ? `${API_BASE}/plots/${selectedPlot}` : null}
        title={selectedPlot}
        zoom={plotZoom}
        onZoomIn={() => setPlotZoom((z) => z + 0.2)}
        onZoomOut={() => setPlotZoom((z) => Math.max(0.4, z - 0.2))}
        onClose={closePlot}
      />
    </div>
  );
}