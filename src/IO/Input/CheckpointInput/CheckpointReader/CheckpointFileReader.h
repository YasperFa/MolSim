//
// Created by Celik Yucel on 12.12.24.
//

#pragma once
#include "IO/Input/CheckpointInput/CheckpointSchema.h"
#include "Objects/Containers/ParticleContainer.h"


class CheckpointFileReader {
public:
    /**
     * @brief prints the metadata for the read checkpoint file belonging to a sub-simulation
     * prints out the name of the  original input file for the sub-simulation, and the chosen deltaT, endTime and gravity
     * @param simulationData: Simulation data extracted from the checkpoint file
     */
    static void printSimulationData(const SimulationDataType &simulationData);


    /**
     * @brief reads the given .chk file and adds the specified particles into the passed particle container
     * @param fileStream: input checkpoint file
     * @param particleContainer: the container where the particles will be saved
     */
    static void readCheckpoint(std::string &fileStream, std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer);
};
