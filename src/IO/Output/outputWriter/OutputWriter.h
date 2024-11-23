//
// Created by Celik Yucel on 08.11.24.
//

#pragma once

#include "../../../Objects/Containers/ParticleContainer.h"

namespace outputWriters {
    class OutputWriter {
    public:
        OutputWriter() = default;
        virtual ~OutputWriter() = default;

        virtual void plotParticles(int iteration, ParticleContainer& particleContainer, const std::string& filename) = 0;
    };
}