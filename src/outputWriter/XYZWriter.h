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

  void plotParticles(std::list<Particle> particles, const std::string &filename,
                     int iteration);

  void plotParticlesFromContainer(ParticleContainer &particles, const std::string &filename, int iteration);
};

} // namespace outputWriter
