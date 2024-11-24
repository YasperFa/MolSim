//
// Created by Yasmine Farah on 05/11/2024.
//

#pragma once

#include "Calculator.h"
#include "Objects/Particle.h"
#include "../Objects/Containers/Direct Sum/DirectSumContainer.h"


namespace Calculators{
    class LennardJonesCalculator : public Calculator {
        public:

          LennardJonesCalculator() = default;

          virtual ~LennardJonesCalculator() = default;
          /**
          *  function for calculating the force FIJ acting between two particles according to lennard jones
          * @param sub: subtraction of positions
          * @param m1: mass of particle i
          * @param m2:mass of particle j
          * @param normCubed: the cube of the norm
          * @return force fij between the particle i and j
          */
        std::array<double, 3> calculateFIJ(const std::array<double,3> &sub, double m1, double m2, double normCubed) override;


    };
}

