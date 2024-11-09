/*
 * VTKWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "Objects/Particle.h"
#include "outputWriter/vtk-unstructured.h"
#include "OutputWriter.h"

#include <list>

namespace outputWriters {
    /**
     * This class implements the functionality to generate vtk output from
     * particles.
     */
    class VTKWriter : public OutputWriter {

    public:
        VTKWriter() = default;

        ~VTKWriter() override = default;

        /**
         * set up internal data structures and prepare to plot a particle.
         */
        void initializeOutput(int numParticles);

        /**
         * plot type, mass, position, velocity and force of a particle.
         *
         * @note: initializeOutput() must have been called before.
         */
        void plotParticle(Particle &p);

        /**
         * writes the final output file.
         *
         * @param filename the base name of the file to be written.
         * @param iteration the number of the current iteration,
         *        which is used to generate an unique filename
         */
        void writeFile(const std::string &filename, int iteration);

        void plotParticles(int iteration, ParticleContainer &particleContainer, const std::string &filename) override;

    private:
        VTKFile_t *vtkFile{};
    };
} // namespace outputWriter
