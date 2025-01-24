//
// Created by Celik Yucel on 15.01.25.
//

#pragma once
#include "../Objects/Particle.h"
#include "../Objects/Containers/DirectSum/DirectSumContainer.h"
#include "../Objects/Containers/LinkedCell/LinkedCellContainer.h"
#include <array>
#include <string>

namespace Calculators {
    class HarmonicForceCalculator {
    private:
        /**
         * Indicates whether the container is a LinkedCellContainer.
         */
        bool isLinkedCellContainer;

        /**
         * The size of the domain (only relevant for LinkedCellContainer).
         */
        std::array<double, 3> domainSize;

    public:
        /**
         * Constructor for the harmonic force calculator.
         * @param container The particle container used in the simulation.
         */
        HarmonicForceCalculator(const ParticleContainers::ParticleContainer& container);

        /**
         * Calculates the total harmonic force acting on the given particle due to its connected neighbors.
         * @param current The particle for which the harmonic force is calculated.
         * @return Total harmonic force as a 3D vector.
         */
        std::array<double, 3> calculateForce(Particle& current) const;

        /**
         * Returns the name of the calculator as a string.
         * @return Name of the calculator.
         */
        std::string toString();
    };
}
