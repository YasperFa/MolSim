/*
 * XYZWriter.cpp
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#include "XYZWriter.h"
#include <iomanip>
#include <sstream>
#include "spdlog/spdlog.h"

namespace outputWriters {
    void XYZWriter::plotParticles(int iteration, ParticleContainer &particleContainer, const std::string &filename) {
        SPDLOG_TRACE("XYZ Writer plotParticlesFromContainer: opening file in iteration {}", iteration);
        std::ofstream file;
        std::stringstream strstr;
        strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration << ".xyz";

        file.open(strstr.str().c_str());
        file << particleContainer.sizeParticles() << std::endl;
        file << "Generated by MolSim. See http://openbabel.org/wiki/XYZ_(format) for "
                "file format doku."
                << std::endl;
        SPDLOG_TRACE("XYZ Writer plotParticlesFromContainer: plotting particles in iteration {}", iteration);
        for (auto it = particleContainer.begin(); it != particleContainer.end(); ++it) {
            const Particle &p = *it;
            std::array<double, 3> x = p.getX();
            file << "Ar ";
            file.setf(std::ios_base::showpoint);

            for (const auto &xi: x) {
                file << xi << " ";
            }
            file << std::endl;
        }
        file.close();
    }
} // namespace outputWriter
