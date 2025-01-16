//
// Created by Celik Yucel on 15.01.25.
//

#include "HarmonicForceCalculator.h"
#include "utils/ArrayUtils.h"
#include <cmath>
#include "spdlog/spdlog.h"

namespace Calculators {
    HarmonicForceCalculator::HarmonicForceCalculator(double stiffnessConstant, double avgBondLength)
        : stiffnessConstant(stiffnessConstant), avgBondLength(avgBondLength) {}

    std::array<double, 3> HarmonicForceCalculator::calculateFIJ(const std::array<double, 3> &sub, Particle &current,
                                                                Particle &other, double boundLengthMultiplier) {
        SPDLOG_TRACE("Executing calculateFIJ with HarmonicForceCalculator");

        double distance = ArrayUtils::L2Norm(sub);
        double bondLength = boundLengthMultiplier * avgBondLength;

        double multiplier = stiffnessConstant * (distance - bondLength) / distance;
        return multiplier * sub;
    }

    std::string HarmonicForceCalculator::toString() {
        return "Harmonic";
    }
}
