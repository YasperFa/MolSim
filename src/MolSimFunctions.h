//
// Created by Celik Yucel on 08.11.24.
//
#pragma once

#include <iostream>

#include "Calculator/Calculator.h"
#include "Calculator/LennardJonesCalculator.h"
#include "IO/Input/FileReader.h"
#include "Objects/Containers/DirectSum/DirectSumContainer.h"
#include "IO/Output/outputWriter/OutputWriter.h"
#include "IO/Output/outputWriter/VTKWriter.h"
#include "IO/Output/outputWriter/XYZWriter.h"
#include "spdlog/spdlog.h"
#include "cxxopts.hpp"
#include "Calculator/GravityCalculator.h"
#include "Objects/Containers/LinkedCell/LinkedCellContainer.h"
#include "Objects/Containers/LinkedCell/BoundaryHandler.h"
#include "Objects/Temperature/Thermostat.h"

/**Provides utility functions for running the simulation */
namespace MolSim {
    /**Prints the help message to std::out */
    void printHelp();

    /**  the arguments from argv and initializes the arguments accordingly
     * @param argc number of arguments
     * @param argv argument vector
     * @param inputFile valid path to input file
     * @param deltaT
     * @param endTime
     * @param gravity
     * @param harmonicOn
     * @param stiffnessConstant
     * @param avgBondLength
     * @param outputWriter that will be used to write the output
     * @param calculator that will be used during the simulation
     * @param particleContainer empty ParticleContainer
     * @param boundaryHandler
     */
    bool parseArguments(int argc, char *argv[], std::string &inputFile, double &deltaT, double &endTime,
                        double &gravity,
                        bool harmonicOn,
                        double stiffnessConstant,
                        double avgBondLength,
                        std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                        std::unique_ptr<Calculators::Calculator> &calculator,
                        std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer,
                        std::unique_ptr<BoundaryHandler> &boundaryHandler);

    /** Runs the simulation using the specified parameters
     * @param particleContainer initialized ParticleContainer
     * @param deltaT
     * @param endTime
     * @param gravity
     * @param harmonicOn
     * @param stiffnessConstant
     * @param avgBondLength
     * @param upwardsForce
     * @param activeTimsteps
     * @param freq
     * @param gravityAxis
     * @param specialForceAxis axis where additional force should be applied
     * @param version2 if true, use parallelization method 2, else use method 1
     * @param outputWriter that will be used to write the output
     * @param calculator that will be used during the simulation
     * @param boundaryHandler that will be used to handle boundaries
     * @param thermostat to adjust the temperature of the simulation
     * @param inputFile
    */
    void runSim(ParticleContainers::ParticleContainer &particleContainer, double &deltaT, double &endTime,
                double &gravity,
                bool harmonicOn,
                double stiffnessConstant,
                double avgBondLength,
                double &upwardsForce, int &activeTimesteps,
                int &freq,
                int &gravityAxis, int &specialForceAxis,
                bool &version2,
                std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                std::unique_ptr<Calculators::Calculator> &calculator,
                std::unique_ptr<BoundaryHandler> &boundaryHandler, std::unique_ptr<Thermostat> &thermostat,
                std::string &inputFile);


    /**
     * @brief runs the sub-simulations specified in the input xml file
     * @param mainInputFile: input file of the main simulation
     * @return true if sub-simulations are ran successfully
     */
    bool runSubSim(std::string &mainInputFile);

    /**
     * @brief checks the current directory and loads every .chk file into the particle container
     * @param particleContainer: the particle container where the particles will be loaded into
     * @return true if loading the checkpoints is successful
     */
    bool loadCheckpoints(std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer);
}
