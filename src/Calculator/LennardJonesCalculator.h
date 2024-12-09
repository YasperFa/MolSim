//
// Created by Yasmine Farah on 05/11/2024.
//

#pragma once

#include "Calculator.h"
#include "Objects/Particle.h"
#include "../Objects/Containers/DirectSum/DirectSumContainer.h"


namespace Calculators{
    class LennardJonesCalculator : public Calculator {
        public:

          LennardJonesCalculator() = default;

          ~LennardJonesCalculator() override = default;
          /**
          *  function for calculating the force FIJ acting between two particles according to lennard jones
          * @param sub: subtraction of positions
          * @param m1: mass of particle i (not used for LennardJonesCalculator)
          * @param m2: mass of particle j (not used for LennardJonesCalculator)
          * @param normCubed: the cube of the norm
          * @param epsilon1 the Lennard-Jones parameter epsilon of i
          * @param epsilon2 the Lennard-Jones parameter epsilon of j
          * @param sigma1 the Lennard-Jones parameter sigma of i
          * @param sigma2 the Lennard-Jones parameter sigma of j
          * @return force fij between the particle i and j
          */
        std::array<double, 3> calculateFIJ(const std::array<double, 3> &sub, double m1, double m2,
                                                   double normCubed, double epsilon1, double epsilon2, double sigma1, double sigma2) override;
        std::string toString() override;

    };
}

