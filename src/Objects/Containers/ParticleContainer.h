//
// Created by Celik Yucel on 23.11.24.
//

#pragma once

#include <functional>
#include "Objects/Particle.h"
namespace ParticleContainers {
    class ParticleContainer {
    public:
        /**
         * @brief Virtual destructor. It will be used by the inheriting classes.
         */
        virtual ~ParticleContainer() = default;

        /**
        * This method adds the passed particle into the container.
        * @param particle is the particle to be added.
        */
        virtual void addParticle(const Particle& particle) = 0;


        /**
         * @return the number of particles in the container
         */
        virtual size_t sizeParticles() const = 0;

        virtual std::vector<Particle> getParticles() const = 0;

        /**
        * @return an iterator to the beginning of the particles vector.
        */
        virtual std::vector<Particle>::iterator begin() = 0;

        /**
        * @return an iterator to the end of the particles vector.
        */
        virtual std::vector<Particle>::iterator end() = 0;

    };
}