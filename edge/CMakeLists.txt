cmake_minimum_required(VERSION 3.16)

# Nome progetto
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
)

# Include directory (pubblica perché usata da chi linka)
target_include_directories(neuraledge_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Warning seri (importantissimo per qualità codice)
target_compile_options(neuraledge_core PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# =========================
# EXECUTABLE
# =========================

add_executable(neuraledge_edge
    src/main.cpp
)

# Link con la libreria core
target_link_libraries(neuraledge_edge
    PRIVATE neuraledge_core
)