//
// Created by Celik Yucel on 23.10.24.
//
#pragma once

#include <vector>
#include <functional>

#include "../../Particle.h"
#include "../ParticleContainer.h"

/**
 * @brief Container for storing Particle objects and
 * to provide iterators implementing the DirectSum algorithm. Extends ParticleContainer.
 */
class DirectSumContainer : public ParticleContainer{

    private:
    /**
    * Vector for storing particles
    */
    std::vector<Particle> particles;

    /**
    * Vector for storing particle pairs
    */
    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>> particlePairs;

    public:
    /**
    * @return an iterator to the beginning of the particles vector.
    */
    std::vector<Particle>::iterator begin() override;

    /**
    * @return an iterator to the end of the particles vector.
    */
    std::vector<Particle>::iterator end() override;


    /**
    * default constructor
    */
    DirectSumContainer() = default;

    /**
    * This method adds the passed particle into the container.
    * No pairs are created. Call initializePairsVector() after adding all particles.
    * @param particle is the particle to be added.
    */
    void addParticle(const Particle& particle) override;

    /**
    * This method adds the passed particle into the container and creates all new pairs containing this particle.
    * Equivalent to calling addParticle() and then calling reinitializePairs(), but more efficient.
    * @param particle is the particle to be added.
    */
    void addParticleToPairs(Particle& particle) override;

    /**
    * This method removes the passed particle from the container.
    * All pairs containing this particle are removed.
    * @param particle is the particle to be removed.
    */
    void removeParticle(const Particle& particle) override;

    /**
     * This method returns the particle with the corresponding id
     * @param id is a unique int identifier
     * @returns the corresponding particle
     * 
     * Throws an error if the container does not contain a particle with this id
     */

    Particle& getParticle(int id);

    /**
     * This method returns the particle from the container with the corresponding id
     * @param p a particle p which exists in the container
     * @returns the corresponding particle
     * 
     * Throws an error if the container does not contain a particle with this id
     */

    Particle& getParticle(Particle& p);


    /**
     * @return the number of particles in the container
     */
    size_t sizeParticles() const override;


    std::vector<Particle> getParticles() const override;

};
