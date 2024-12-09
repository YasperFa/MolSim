//
// Created by Yasmine Farah on 10/11/2024.
//

#pragma once
#include "Shapes/Cuboid.h"
#include "Shapes/Disc.h"
#include "Containers/ParticleContainer.h"

/**class that generates particles from a given cuboid or disc */
class ParticleGenerator {

public:
   ParticleGenerator() = default;
   ~ParticleGenerator() = default;
   /**
   * @brief generates particles from a given cuboid
   * @param particles empty Particle container where particles will be inserted
   * @param cuboid Cuboid that defines parameters
   * @param type defines the type of the particles
   */
   static void generateCuboid(ParticleContainers::ParticleContainer &particles, Cuboid& cuboid, int type, double epsilon, double sigma);
   /**
   * @brief generates particles from a given disc
   * @param particles empty Particle container where particles will be inserted
   * @param disc Disc that defines parameters
   * @param type defines the type of the particles
   */
   static void generateDisc(ParticleContainers::ParticleContainer &particles, Disc& disc, int type, double epsilon, double sigma);
};


