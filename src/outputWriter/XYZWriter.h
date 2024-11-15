/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "Particle.h"

#include <fstream>
#include <list>

#include "ParticleContainer.h"

namespace outputWriter {

class XYZWriter {

public:
  XYZWriter();

  virtual ~XYZWriter();

  /**Plots particles from a particle list
 * @param particles: the particle container
 * @param filename of file where output should be written
 * @param iteration
 */

  void plotParticles(std::list<Particle> particles, const std::string &filename,
                     int iteration);

  
/**Plots particles from a particle container
 * @param particles: the particle container
 * @param filename of file where output should be written
 * @param iteration
 */
  void plotParticlesFromContainer(ParticleContainer &particles, const std::string &filename, int iteration);
};

} // namespace outputWriter
