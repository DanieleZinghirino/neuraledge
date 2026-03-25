cmake_minimum_required(VERSION 3.16)

# =========================
# PROJECT
# =========================
project(neuraledge_edge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dipendenza esterna per la configurazione YAML
find_package(yaml-cpp REQUIRED)

# =========================
# INTERFACE LIBRARY — opzioni condivise da tutti i target
# =========================
add_library(project_options INTERFACE)

target_compile_options(project_options INTERFACE
    -Wall
    -Wextra
    -Wpedantic
)

target_include_directories(project_options INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# =========================
# HELPER MACRO
# =========================
macro(make_edge_lib NAME)
    add_library(${NAME} ${ARGN})
    target_link_libraries(${NAME} PUBLIC project_options)
endmacro()

# =========================
# LIBRERIE CORE
# =========================
make_edge_lib(edge_signal
    src/signal/TemperatureSignal.cpp
    src/signal/CurrentSignal.cpp
    src/signal/SpeedSignal.cpp
    src/signal/VibrationSignal.cpp
    src/signal/PressureSignal.cpp
    src/signal/HumiditySignal.cpp
    src/signal/SquareWaveSignal.cpp
    src/signal/RampSignal.cpp
    src/signal/StepResponseSignal.cpp
)

make_edge_lib(edge_noise
    src/noise/GaussianNoise.cpp
    src/noise/UniformNoise.cpp
    src/noise/PinkNoise.cpp
    src/noise/DriftNoise.cpp
    src/noise/PeriodicNoise.cpp
    src/noise/ImpulseNoise.cpp
)

make_edge_lib(edge_fault
    src/fault/NoFault.cpp
    src/fault/SpikeFault.cpp
    src/fault/StuckFault.cpp
    src/fault/DriftFault.cpp
    src/fault/ScalingFault.cpp
    src/fault/SaturationFault.cpp
    src/fault/DropoutFault.cpp
)

make_edge_lib(edge_sensor
    src/sensor/Sensor.cpp
)

make_edge_lib(edge_logger
    src/logger/CsvLogger.cpp
)

make_edge_lib(edge_runner
    src/runner/SensorRunner.cpp
)

make_edge_lib(edge_factory
    src/factory/SensorFactory.cpp
)

make_edge_lib(edge_config
    src/config/ConfigLoader.cpp
)

# =========================
# DIPENDENZE ESPLICITE
# =========================
target_link_libraries(edge_sensor PRIVATE
    edge_signal
    edge_noise
    edge_fault
)

target_link_libraries(edge_runner PRIVATE
    edge_sensor
    edge_logger
)

target_link_libraries(edge_factory PRIVATE
    edge_sensor
    edge_signal
    edge_noise
    edge_fault
)

target_link_libraries(edge_config PRIVATE 
    yaml-cpp::yaml-cpp
)

# =========================
# EXECUTABLE
# =========================
add_executable(edge
    src/main.cpp
)

target_link_libraries(edge PRIVATE
    edge_sensor
    edge_logger
    edge_runner
    edge_factory
    edge_config
)