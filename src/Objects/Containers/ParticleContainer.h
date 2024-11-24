//
// Created by Celik Yucel on 23.11.24.
//

#pragma once

#include <functional>
#include "Objects/Particle.h"

class ParticleContainer {
    public:
    /**
     * @brief Virtual destructor. It will be used by the inheriting classes.
     */
    virtual ~ParticleContainer() = default;

    /**
    * This method adds the passed particle into the container.
    * No pairs are created. Call initializePairsVector() after adding all particles.
    * @param particle is the particle to be added.
    */
    virtual void addParticle(const Particle& particle) = 0;

    /**
    * This method adds the passed particle into the container and creates all new pairs containing this particle.
    * Equivalent to calling addParticle() and then calling reinitializePairs(), but more efficient.
    * @param particle is the particle to be added.
    */
    virtual void addParticleToPairs(Particle& particle) = 0;

    /**
    * This method removes the passed particle from the container.
    * All pairs containing this particle are removed.
    * @param particle is the particle to be removed.
    */
    virtual void removeParticle(const Particle& particle) = 0;

    /**
    * This method checks if a pair containing both particles passed as arguments
    * already exits in the particlePairs vector.
    *
    * @param particle1
    * @param particle2
    * @returns true if pair already exists in the particlePairs vector
    */
    virtual bool pairExists(const Particle& particle1, const Particle& particle2) const = 0;

    /**
    * This method creates pairs for every particle in the container
    * and adds these pairs to particlesPairs vector.
    */
    virtual void initializePairsVector() = 0;

    /**
    * This method clears the particlePairs vector and initializes
    * the pairs once again.
    * This method is to be used after removing a particle from the container.
    */
    virtual void reinitializePairs() = 0;

    /**
     * @return the number of particles in the container
     */
    virtual size_t sizeParticles() const = 0;

    /**
    * @return the number of particle pairs in the container
    */
    virtual size_t sizeParticlePairs() const = 0;


    virtual std::vector<Particle> getParticles() const = 0;
    virtual std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>> getParticlePairs() const = 0;

};
