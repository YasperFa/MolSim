//
// Created by Celik Yucel on 08.11.24.
//

#pragma once

#include "../../../Objects/Containers/Direct Sum/DirectSumContainer.h"

namespace outputWriters {
    class OutputWriter {
    public:
        OutputWriter() = default;
        virtual ~OutputWriter() = default;

        virtual void plotParticles(int iteration, DirectSumContainer& particleContainer, const std::string& filename) = 0;
    };
}