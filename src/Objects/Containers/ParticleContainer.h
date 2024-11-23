//
// Created by Celik Yucel on 23.10.24.
//
#pragma once

#include "../Particle.h"

#include <vector>
#include <functional> 

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


    /**
    * default constructor
    */
    ParticleContainer() = default;

    /**
     * default deconstructor
     */
    ~ParticleContainer() = default;


    /**
    * This method adds the passed particle into the container.
    * No pairs are created. Call initializePairsVector() after adding all particles.
    * @param particle is the particle to be added.
    */
    void addParticle(const Particle& particle);

    /**
    * This method adds the passed particle into the container and creates all new pairs containing this particle.
    * Equivalent to calling addParticle() and then calling reinitializePairs(), but more efficient.
    * @param particle is the particle to be added.
    */
    void addParticleToPairs(Particle& particle);

    /**
    * This method removes the passed particle from the container.
    * All pairs containing this particle are removed.
    * @param particle is the particle to be removed.
    */
    void removeParticle(const Particle& particle);

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
     * @param a particle p which exists in the container
     * @returns the corresponding particle
     * 
     * Throws an error if the container does not contain a particle with this id
     */

    Particle& getParticle(Particle& p);

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


    std::vector<Particle> getParticles() const;
    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>> getParticlePairs() const;

};
