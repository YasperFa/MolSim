/*
* Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>
#include "ParticleIdInitializer.h"
#include <mutex>
class Particle {

private:
 mutable std::mutex forceMutex;
 /**
  * Position of the particle
  */
 std::array<double, 3> x;

  /**
  * former position of the particle
  */
 std::array<double, 3> old_x;

 /**
  * Velocity of the particle
  */
 std::array<double, 3> v;

 /**
  * Force effective on this particle
  */
 std::array<double, 3> f;

 /**
  * Force which was effective on this particle
  */
 std::array<double, 3> old_f;

 /**
  * Mass of this particle
  */
 double m;

 /**
  * Type of the particle. Use it for whatever you want (e.g. to separate
  * molecules belonging to different bodies, matters, and so on)
  */
 int type;

 /** Id of the particle. New particles are created with ascending id numbers starting with 1.
  *  Every id is unique. 
 */

int id;

/**Lennard-Jones parameter epsilon */
double epsilon;

/**Lennard-Jones parameter sigma */
double sigma;

/**Id of clone particles in x, y, and z direction for periodic boundaries
 * Value of 0 if there is no partner
 */
std::array<int, 3> partner;

public:
 explicit Particle(int type = 0);

 Particle(const Particle &other);

 Particle(
     // for visualization, we need always 3 coordinates
     // -> in case of 2d, we use only the first and the second
     std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
     int type = 0, double epsilon_arg = 5, double sigma_arg = 1);

 virtual ~Particle();

 const std::array<double, 3> &getX() const;

 const std::array<double, 3> &getV() const;

 const std::array<double, 3> &getF() const;

 const std::array<double, 3> &getOldF() const;

 const std::array<double, 3> &getOldX() const;

 double getEpsilon();

 double getSigma();

 int getID() const;

 double getM() const;

 int getType() const;

 void setF(const std::array<double, 3> &newF);
 void setOldF(const std::array<double, 3> &newOldF);
 void setX(const std::array<double, 3> &X);
 void setOldX(const std::array<double, 3> &newOldX);
 void setV(const std::array<double, 3> &V);
 bool operator==(Particle &other);
 bool operator==(const Particle &other);
 Particle& operator=(const Particle& other);

/** Set the clone partner particle in x, y or z direction
 * @param i int to show the direction: 0 and 1 for x direction, 2 and 3 for y direction, 4 and 5 for z direction
 * @param id the id of the partner
 */
 void setPartner(int i, int id);

 /** Retrieve the clone partner particle in x, y or z direction
  * It is possible that the returned value is no longer valid, the existence of the partner must be checked
 * @param i int to show the direction: 0 and 1 for x direction, 2 and 3 for y direction, 4 and 5 for z direction
 * @returns id of the corresponding partner, 0 if there is no partner
 */
 int getPartner(int i);

 std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
