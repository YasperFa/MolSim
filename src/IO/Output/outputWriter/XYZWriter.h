/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "Objects/Particle.h"
#include "OutputWriter.h"

#include <fstream>
#include <list>

#include "Objects/ParticleContainer.h"

namespace outputWriters {
    class XYZWriter : public OutputWriter {
    public:
        XYZWriter() = default;

        virtual ~XYZWriter() = default;

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
        void plotParticles(int iteration, ParticleContainer &particleContainer, const std::string &filename) override;
    };
} // namespace outputWriter
