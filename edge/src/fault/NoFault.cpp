#include "fault/NoFault.hpp"

double NoFault::apply(double value) {
    return value; // nessun guasto, nessuna perturbazione
}
