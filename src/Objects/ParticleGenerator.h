//
// Created by Yasmine Farah on 10/11/2024.
//

#pragma once
#include "Cuboid.h"
#include "Disc.h"
/**class that generates particles from a given cuboid or disc */
class ParticleGenerator {

public:
   ParticleGenerator() = default;
   ~ParticleGenerator() = default;
   /**
   * @brief generates particles from a given cuboid
   * @param particles empty Particle container where particles will be inserted
   * @param cuboid Cuboid that defines parameters
   */
   static void generateCuboid(ParticleContainers::ParticleContainer &particles, Cuboid& cuboid);
   /**
   * @brief generates particles from a given disc
   * @param particles empty Particle container where particles will be inserted
   * @param disc Disc that defines parameters
   */
   static void generateDisc(ParticleContainers::ParticleContainer &particles, Disc& disc);
};


