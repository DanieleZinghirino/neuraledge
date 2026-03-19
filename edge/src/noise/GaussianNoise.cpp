#include "noise/GaussianNoise.hpp"

GaussianNoise::GaussianNoise(double sigma): 
    gen_(std::random_device{}()), 
    dist_(0.0, sigma) {}

double GaussianNoise::apply() {
    return dist_(gen_);
}
