#include "noise/UniformNoise.hpp"

UniformNoise::UniformNoise(double amplitude)
    : gen_(std::random_device{}()),
      dist_(-amplitude, amplitude) {}

double UniformNoise::apply() {
    return dist_(gen_);
}