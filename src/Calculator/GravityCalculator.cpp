//
// Created by Yasmine Farah on 26/11/2024.
//

#include "GravityCalculator.h"
namespace Calculators {
    std::array<double, 3> GravityCalculator::calculateFIJ(const std::array<double,3> &sub, double m1, double m2, double normCubed) {
        return operator*(1.0/pow(normCubed, 3), operator*(m1*m2, sub));
    }
}