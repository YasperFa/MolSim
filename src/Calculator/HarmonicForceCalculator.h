//
// Created by Celik Yucel on 15.01.25.
//

#pragma once
#include "../Objects/Particle.h"
#include <array>
#include <string>

namespace Calculators {
    class HarmonicForceCalculator {
    private:
        double stiffnessConstant;
        double avgBondLength;

    public:
        HarmonicForceCalculator(double stiffnessConstant, double avgBondLength);
        std::array<double,3> calculateFIJ(const std::array<double,3>& sub, Particle& current, Particle& other, double boundLengthMultiplier);
        std::string toString();
    };
}
