//
// Created by Yasmine Farah on 05/11/2024.
//
#include "../Objects/Particle.h"
#include "LennardJonesCalculator.h"
#include "utils/ArrayUtils.h"
#include <cmath>
#include "spdlog/spdlog.h"

double cutOffValue =  std::pow(2.0, 1.0 / 6.0); //precompute value

namespace Calculators {
    std::array<double, 3> LennardJonesCalculator::calculateFIJ(const std::array<double, 3> &sub, double m1, double m2,
                                                               double normCubed, double epsilon1, double epsilon2,
                                                               double sigma1, double sigma2) {
        SPDLOG_TRACE("executing calculateF with lennardJonesCalculator");

        double smallSigma = (sigma1 == sigma2) ? sigma1 : (sigma1 + sigma2) / 2.0;

        // cutoff = std::pow(2.0, 1.0 / 6.0) * smallSigma
        if (repulsiveOnly && normCubed > cutOffValue * smallSigma) { //only evaluate cutoff if necessary
            return {0.0, 0.0, 0.0};
        }

        double epsilon = (epsilon1 == epsilon2) ? epsilon1 : sqrt(epsilon1 * epsilon2);
        double potential_dirivative = -24 * epsilon * (pow((smallSigma / normCubed), 6) - 2 * pow(
                                                           (smallSigma / normCubed), 12)) / pow(normCubed, 2);
        const std::array<double, 3> fij = (potential_dirivative * (-1 * sub));
        return fij;
    }

    std::string LennardJonesCalculator::toString() {
        return "LJC";
    }
}
