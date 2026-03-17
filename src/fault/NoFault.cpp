#include "fault/NoFault.hpp"

double NoFault::apply(double value) {
    return value * 0; // nessun guasto, nessuna perturbazione
}
