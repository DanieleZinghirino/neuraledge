cmake_minimum_required(VERSION 3.16)

# =========================
# PROJECT
# =========================
project(neuraledge_edge LANGUAGES CXX)

# Standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================
# LIBRERIA CORE
# =========================

add_library(neuraledge_core

    # Sensor
    src/sensor/Sensor.cpp

    # Signal
    src/signal/TemperatureSignal.cpp

    # Noise
    src/noise/GaussianNoise.cpp

    # Fault
    src/fault/NoFault.cpp
    src/fault/SpikeFault.cpp

    # 🔥 NEW: Logger
    src/logger/CsvLogger.cpp

    # 🔥 NEW: Runner
    src/runner/SensorRunner.cpp
)

# Include directories
target_include_directories(neuraledge_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Warning seri (qualità codice)
target_compile_options(neuraledge_core PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# =========================
# EXECUTABLE
# =========================

add_executable(edge
    src/main.cpp
)

# Link con la libreria core
target_link_libraries(edge
    PRIVATE neuraledge_core
)