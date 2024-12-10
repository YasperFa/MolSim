//
// Created by Yasmine Farah on 05/11/2024.
//
#include "../Objects/Particle.h"
#include "LennardJonesCalculator.h"
#include "utils/ArrayUtils.h"
#include <cmath>
#include "spdlog/spdlog.h"


namespace Calculators {

    std::array<double, 3> LennardJonesCalculator::calculateFIJ(const std::array<double,3> &sub, double m1, double m2, double normCubed, double epsilon1, double epsilon2, double sigma1, double sigma2) {
        SPDLOG_TRACE("executing calculateF with lennardJonesCalculator");

        double smallSigma;
        double epsilon;

        if (sigma1 == sigma2) { //perform check for higher efficiency (avoid calculation if possible)
            smallSigma = sigma1;
        } else {
        smallSigma = (sigma1 + sigma2)/2.0;
        }

        if (epsilon1 == epsilon2){
            epsilon = epsilon1;
        } else {
        epsilon = sqrt(epsilon1 * epsilon2);
        }

        double potential_dirivative = -24*epsilon*(pow((smallSigma/normCubed),6) - 2*pow((smallSigma/normCubed),12))/pow(normCubed, 2);
        const std::array<double,3> fij = operator*(potential_dirivative, operator*(-1,sub));
        return fij;
    }
    std::string LennardJonesCalculator::toString() {
        return "LJC";
    }
}






