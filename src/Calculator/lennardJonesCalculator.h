//
// Created by Yasmine Farah on 05/11/2024.
//

#ifndef LENNARDJONESCALCULATOR_H
#define LENNARDJONESCALCULATOR_H
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
          * @param ParticleContainer
          */
          void calculateF(ParticleContainer &particleContainer) override;


    };
}


#endif //LENNARDJONESCALCULATOR_H
