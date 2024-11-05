//
// Created by Yasmine Farah on 05/11/2024.
//

#pragma once

#include "Calculator.h"
#include "Objects/ParticleContainer.h"
#include <Objects/Particle.h>
namespace Calculators{

    class lennardJonesCalculator : public Calculator {
        public:

          lennardJonesCalculator() = default;

          virtual ~lennardJonesCalculator() = default;
          /**
          *  function for calculating the force F acting between particles
          * @param particleContainer
          */
          void calculateF(ParticleContainer &particleContainer) override;


    };
}

