//
// Created by Yasmine Farah on 26/11/2024.
//

#pragma once
#include "Calculator.h"

namespace Calculators {
    class GravityCalculator : public Calculator {
    public:
        GravityCalculator() = default;

        ~GravityCalculator() override = default;

        /**
        *  function for calculating the force FIJ acting between two particles according to newton's law
        * @param sub: subtraction of positions
        * @param m1: mass of particle i
        * @param m2:mass of particle j
        * @param normCubed: the cube of the norm
        * @param epsilon1 the Lennard-Jones parameter epsilon of i (not used for gravity calculator)
        * @param epsilon2 the Lennard-Jones parameter epsilon of j (not used for gravity calculator)
        * @param sigma1 the Lennard-Jones parameter sigma of i (not used for gravity calculator)
        * @param sigma2 the Lennard-Jones parameter sigma of j (not used for gravity calculator)
        * @return force fij between the particle i and j
        */
        std::array<double, 3> calculateFIJ(const std::array<double, 3> &sub, double m1, double m2,
                                           double normCubed, double epsilon1, double epsilon2, double sigma1,
                                           double sigma2, bool neighbours) override;

        std::string toString() override;
    };
};
