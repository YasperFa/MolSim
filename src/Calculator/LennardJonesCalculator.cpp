//
// Created by Yasmine Farah on 05/11/2024.
//
#include "../Objects/Particle.h"
#include "LennardJonesCalculator.h"
#include "utils/ArrayUtils.h"
#include <cmath>
#include "spdlog/spdlog.h"


namespace Calculators {
    static const double epsilon = 5;
    static const double smallSigma = 1;
    std::array<double, 3> LennardJonesCalculator::calculateFIJ(const std::array<double,3> &sub, double m1, double m2, double normCubed) {
        SPDLOG_TRACE("executing calculateF with lennardJonesCalculator");
        double potential_dirivative = -24*epsilon*(pow((smallSigma/normCubed),6) - 2*pow((smallSigma/normCubed),12))/pow(normCubed, 2);
        const std::array<double,3> fij = operator*(potential_dirivative, operator*(-1,sub));
        return fij;
    }
    std::string LennardJonesCalculator::toString() {
        return "LJC";
    }
}






