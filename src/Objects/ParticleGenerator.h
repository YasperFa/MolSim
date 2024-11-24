//
// Created by Yasmine Farah on 10/11/2024.
//

#pragma once
#include "Cuboid.h"
#include "Disc.h"
class ParticleGenerator {



public:
   ParticleGenerator() = default;
   ~ParticleGenerator() = default;
   /**
   * @brief generates particles from a given cuboid
   */
   static void generateCuboid(DirectSumContainer &particles, Cuboid& cuboid);
   /**
   * @brief generates particles from a given disc
   */
   static void generateDisc(DirectSumContainer &particles, Disc& disc);
};


