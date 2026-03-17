#include "fault/SpikeFault.hpp"

SpikeFault::SpikeFault(double prob, double mag): 
    probability_(prob), 
    magnitude_(mag), 
    gen_(std::random_device{}()), 
    dist_(0.0, 1.0) {}

double SpikeFault::apply(double value) {
    if (dist_(gen_) < probability_) {
        return value + magnitude_;
    }
    return value;
}
