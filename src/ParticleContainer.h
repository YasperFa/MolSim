//
// Created by Celik Yucel on 23.10.24.
//
#pragma once

#include "Particle.h"

#include <vector>

/**
 * Container for storing Particle objects and
 * to provide iterators.
 */
class ParticleContainer {

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
    /////////////////////////////////////////////(Iterators)///////////////////////////////////////////////////////////
    /**
    * @return an iterator to the beginning of the particles vector.
    */
    std::vector<Particle>::iterator begin();

    /**
    * @return an iterator to the end of the particles vector.
    */
    std::vector<Particle>::iterator end();

    /**
    * @return an iterator to the beginning of the
    * vector storing the particle pairs.
    */
    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>>::iterator begin_pairs();
    /**
    * @return an iterator to the end of the
    * vector storing the particle pairs.
    */
    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>>::iterator end_pairs();


    ///////////////////////////////////(Constructor and Deconstructor)//////////////////////////////////////////////////
    /**
    * default constructor
    */
    ParticleContainer() = default;

    /**
     * default deconstructor
     */
    ~ParticleContainer() = default;

    //////////////////////////////////////////////(Methods)/////////////////////////////////////////////////////////////

    /**
    * This method adds the passed particle into the container.
    * @param particle is the particle to be added.
    */
    void addParticle(const Particle& particle);

    /**
    * This method removes the passed particle from the container.
    * @param particle is the particle to be removed.
    */
    void removeParticle(const Particle& particle);


    /**
    * This method checks if a pair containing both particles passed as arguments
    * already exits in the particlePairs vector.
    * 
    * @param particle1
    * @param particle2
    * @returns true if pair already exists in the particlePairs vector
    */
    bool pairExists(const Particle& particle1, const Particle& particle2) const;


    /**
    * This method creates pairs for every particle in the container
    * and adds these pairs to particlesPairs vector.
    */
    void initializePairsVector();

    /**
    * This method clears the particlePairs vector and initializes
    * the pairs once again.
    * This method is to be used after removing a particle from the container.
    */
    void reinitializePairs();

    /**
     * @return the number of particles in the container
     */
    size_t sizeParticles() const;

    /**
    * @return the number of particle pairs in the container
    */
    size_t sizeParticlePairs() const;


    //////////////////////////////////////////////(Getters)/////////////////////////////////////////////////////////////
    std::vector<Particle> getParticles() const;
    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>> getParticlePairs() const;

};
