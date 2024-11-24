//
// Created by Sawsan Farah on 03/11/2024.
//

#ifndef CUBOID_H
#define CUBOID_H
#include <array>

#include "Containers/Direct Sum/DirectSumContainer.h"

/**
 * a class that defines cuboid parameters
 */
class Cuboid {
private:
 /**
  * Coordinate of the  lower left front-side corner
  */
 const std::array<double, 3> x;
 /**
  * Number of Particles per dimension
  */
 const std::array<double, 3> N;
 /**
  * Distance between particles
  */
 const double h;
 /**
 * Mass of one particle
 */
 const double m;
 /**
 * initial velocity of the particles
 */
 const std::array<double, 3> v;
 /**
 * Mean velocity of the particles in the cuboid.
 */
 const double mv;
    public:
    /**
    * @brief Constructor with initializer list.
    * @param x Lower left front corner of the cuboid.
    * @param N Number of particles per dimension in the cuboid.
    * @param h Distance between particles.
    * @param m Mass of the particles.
    * @param v Initial velocity of the particles.
    * @param mv Mean velocity of the particles.
    */
    Cuboid(const std::array<double, 3> x, const std::array<double, 3> N, const double h, const double m, const std::array<double, 3> v
    ,const double mv) : x(x), N(N), h(h), m(m), v(v), mv(mv){}

    /**
    * @brief default destructor of the cuboid.
    */
     ~Cuboid() = default;
     /**
     * @brief Get the lower left front corner of the cuboid.
     * @return The lower left front corner of the cuboid.
     */
    std::array<double, 3> getLowerFrontCorner() const {
     return x;
    }
    /**
    * @brief Get the number of particles per dimension in the cuboid.
    * @return the number of particles per dimension in the cuboid.
    */
    std::array<double, 3> getNumOfParticlesPerDimension() const {
     return N;
    }
     /**
     * @brief Get the distance between particles in the cuboid.
     * @return the distance between particles in the cuboid.
     */
    double getDistBetweenParticles() const {
     return h;
    }
    /**
    * @brief Get the mass of particles in the cuboid.
    * @return the mass of particles in the cuboid.
    */
    double getMass() const {
     return m;
    }
    /**
   * @brief Get the initial velocity of particles in the cuboid.
   * @return the initial velocity of particles in the cuboid.
   */
    std::array<double, 3> getInitVelocity() const {
     return v;
    }
     /**
    * @brief Get the mean velocity of particles in the cuboid.
    * @return the mean velocity of particles in the cuboid.
    */
    double  getMeanVelocity() const {
     return mv;
    }

};



#endif //CUBOID_H
