//
// Created by Yasmine Farah on 05/11/2024.
//
#include "../Objects/Particle.h"
#include "lennardJonesCalculator.h"
#include "utils/ArrayUtils.h"
#include <cmath>
#include "spdlog/spdlog.h"


namespace Calculators {
    static const double epsilon = 5;
    static const double small_sigma = 1;
    std::array<double, 3> lennardJonesCalculator::calculateFIJ(const std::array<double,3> &sub, double m1, double m2, double normCubed) {
        SPDLOG_TRACE("executing calculateF with lennardJonesCalculator");
        double potential_dirivative = -24*epsilon*(pow((small_sigma/normCubed),6) - 2*pow((small_sigma/normCubed),12))/normCubed;
        std::array<double,3> fij = multiply_constant_vector(sub,potential_dirivative/normCubed);
        return fij;
    }
}






