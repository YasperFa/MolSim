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

class Particle {

private:
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
  *  The ids of shadow particles are the negated ids of their corresponding particles in the boundary
 */

int id;

public:
 explicit Particle(int type = 0);

 Particle(const Particle &other);

 Particle(
     // for visualization, we need always 3 coordinates
     // -> in case of 2d, we use only the first and the second
     std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
     int type = 0);

 virtual ~Particle();

 const std::array<double, 3> &getX() const;

 const std::array<double, 3> &getV() const;

 const std::array<double, 3> &getF() const;

 const std::array<double, 3> &getOldF() const;

 const std::array<double, 3> &getOldX() const;

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

 std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
