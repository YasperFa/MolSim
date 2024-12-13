//
// Created by Celik Yucel on 12.12.24.
//

#pragma once
#include "Objects/Containers/ParticleContainer.h"

class CheckpointFileReader {
public:
    static void readCheckpoint(std::string &fileStream, std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer);
};
