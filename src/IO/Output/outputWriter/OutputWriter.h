//
// Created by Celik Yucel on 08.11.24.
//

#pragma once

#include "../../../Objects/Containers/DirectSum/DirectSumContainer.h"
#include "../../../Objects/Containers/LinkedCell/LinkedCellContainer.h"

namespace outputWriters {
    class OutputWriter {
    public:
        OutputWriter() = default;
        virtual ~OutputWriter() = default;

        virtual void plotParticles(int iteration, ParticleContainers::ParticleContainer& particleContainer, const std::string& filename, const std::string input,
            double &endTime, double& gravity, double &deltaT) = 0;
        virtual std::string toString() = 0;

    };
}