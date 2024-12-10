//
// Created by Celik Yucel on 23.10.24.
//
#pragma once

#include <vector>
#include <functional>

#include "../../Particle.h"
#include "../ParticleContainer.h"

namespace ParticleContainers {
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
  * @param particle is the particle to be added.
  */
  void addParticle(const Particle& particle) override;

  /**
  * This method removes the passed particle from the container.
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

  const Particle& getParticle(int id);

  /**
   * This method returns the particle from the container with the corresponding id
   * @param p a particle p which exists in the container
   * @returns the corresponding particle
   *
   * Throws an error if the container does not contain a particle with this id
   */

  const Particle& getParticle(Particle& p);


  /**
   * @return the number of particles in the container
   */
  size_t sizeParticles() const override;


  std::vector<Particle> getParticles() const override;

 };
}
