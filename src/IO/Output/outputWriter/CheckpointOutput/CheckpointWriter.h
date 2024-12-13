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

        void plotParticles(int iteration, ParticleContainers::ParticleContainer& particleContainer, const std::string& filename, const std::string input,
        double &endTime, double& gravity, double &deltaT) override;

        std::string toString() override;
    };
}
