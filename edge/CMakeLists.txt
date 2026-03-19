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

add_library(edge_signal
    src/signal/CurrentSignal.cpp
    src/signal/SpeedSignal.cpp
    src/signal/TemperatureSignal.cpp)

add_library(edge_noise
    src/noise/GaussianNoise.cpp)

add_library(edge_fault
    src/fault/NoFault.cpp
    src/fault/SpikeFault.cpp)

add_library(edge_sensor
    src/sensor/Sensor.cpp)

add_library(edge_logger
    src/logger/CsvLogger.cpp)

add_library(edge_runner
    src/runner/SensorRunner.cpp)

# Include directories
target_include_directories(edge_signal
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(edge_noise
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(edge_fault
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(edge_sensor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(edge_logger
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(edge_runner
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Warning seri (qualità codice)
target_compile_options(edge_signal PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
target_compile_options(edge_noise PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
target_compile_options(edge_fault PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
target_compile_options(edge_sensor PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
target_compile_options(edge_logger PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
target_compile_options(edge_runner PRIVATE
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
    PRIVATE edge_signal
    PRIVATE edge_noise
    PRIVATE edge_fault
    PRIVATE edge_sensor
    PRIVATE edge_logger
    PRIVATE edge_runner
)