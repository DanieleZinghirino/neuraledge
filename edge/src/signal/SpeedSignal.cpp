#include "signal/SpeedSignal.hpp"

SpeedSignal::SpeedSignal(double v0, double acc)
    : v0_(v0), acceleration_(acc) {}

double SpeedSignal::generate(double t) {
    return v0_ + acceleration_ * t;
}