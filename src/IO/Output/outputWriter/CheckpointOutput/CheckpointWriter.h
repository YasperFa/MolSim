//
// Created by Celik Yucel on 10.12.24.
//

#pragma once
#include "IO/Output/outputWriter/OutputWriter.h"

namespace outputWriters {
    class CheckpointWriter : public OutputWriter {
        public:
        CheckpointWriter() = default;
        ~CheckpointWriter() override = default;

        /**
         * @brief Iterates through the particle container and plots the particle information into a .chk file as specified in the CheckpointSchema.xsd
         * @param iteration: current iteration
         * @param particleContainer: the container where the particles will be read from
         * @param filename: naming scheme for the output files
         * @param input: name of the input file for the simulation
         * @param endTime: end time
         * @param gravity: gravity
         * @param deltaT: delta t
         */
        void plotParticles(int iteration, ParticleContainers::ParticleContainer& particleContainer, const std::string& filename, const std::string input,
                           double &endTime, double& gravity, double &deltaT) override;

        /**
         * 
         * @return the type of output writer (in this case "CheckpointWriter")
         */
        std::string toString() override;
    };
}
