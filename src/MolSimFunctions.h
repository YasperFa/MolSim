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

/**Parses the arguments from argv and initializes the arguments accordingly
 * @param argc number of arguments
 * @param argv argument vector
 * @param inputFile valid path to input file
 * @param deltaT
 * @param endTime
 * @param outputWriter that will be used to write the output
 * @param calculator that will be used during the simulation
 * @particleContainer empty ParticleContainer
 */
bool parseArguments(int argc, char *argv[], std::string &inputFile, double &deltaT, double &endTime,
                            std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                            std::unique_ptr<Calculators::Calculator> &calculator,
                            std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer,
                            std::unique_ptr<BoundaryHandler> &boundaryHandler);

/**Runs the simulation using the specified parameters 
 * @particleContainer initialized ParticleContainer
 * @param deltaT
 * @param endTime
 * @param outputWriter that will be used to write the output
 * @param calculator that will be used during the simulation
*/
void runSim(ParticleContainers::ParticleContainer &particleContainer, double &deltaT, double &endTime, int &freq,
                    std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                    std::unique_ptr<Calculators::Calculator> &calculator,
                    std::unique_ptr<BoundaryHandler> &boundaryHandler,  std::unique_ptr<Thermostat> &thermostat);
}