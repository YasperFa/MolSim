/*
 * XYZWriter.cpp
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#include "outputWriter/XYZWriter.h"
#include <iomanip>
#include <sstream>

#include "ParticleContainer.h"

namespace outputWriter {

XYZWriter::XYZWriter() = default;

XYZWriter::~XYZWriter() = default;

void XYZWriter::plotParticles(std::list<Particle> particles,
                              const std::string &filename, int iteration) {
  std::ofstream file;
  std::stringstream strstr;
  strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration << ".xyz";

  file.open(strstr.str().c_str());
  file << particles.size() << std::endl;
  file << "Generated by MolSim. See http://openbabel.org/wiki/XYZ_(format) for "
          "file format doku."
       << std::endl;

  for (auto &p : particles) {
    std::array<double, 3> x = p.getX();
    file << "Ar ";
    file.setf(std::ios_base::showpoint);

    for (auto &xi : x) {
      file << xi << " ";
    }

    file << std::endl;
  }

  file.close();
}

  void XYZWriter::plotParticlesFromContainer(ParticleContainer& particles, const std::string &filename, int iteration) {
  std::ofstream file;
  std::stringstream strstr;
  strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration << ".xyz";

  file.open(strstr.str().c_str());
  file << particles.sizeParticles() << std::endl;
  file << "Generated by MolSim. See http://openbabel.org/wiki/XYZ_(format) for "
          "file format doku."
       << std::endl;

  for (auto it = particles.begin(); it != particles.end(); ++it) {
    const Particle &p = *it;
    std::array<double, 3> x = p.getX();
    file << "Ar ";
    file.setf(std::ios_base::showpoint);

    for (const auto &xi : x) {
      file << xi << " ";
    }
    file << std::endl;
  }
  file.close();

}

} // namespace outputWriter
