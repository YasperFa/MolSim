#include <iostream>
#include <list>
#include <functional>

#include "MolSim.h"


/** \brief Reads programme input and writes output file
 *
 * @param int argc
 * @param char* argsv[]
 *
 * Takes an input file path and optional parameters and flags to do particle simulation and write the output to a .vtu or .xyz file.
 *
 * Optional parameters/flags:
 * - deltaT value (-d <double value>)
 * - endTime (-e <double value>)
 * - vtk output (-o VTK)
 * - xyz output (-o XYZ)
 * - logLevel (-l {logLevel})
 * - calculator (-c {calculator})
 *
 * Returns 1 if there is no input file or wrong parameters are used.
 *
 *
*/


int main(int argc, char *argsv[]) {
    spdlog::set_level(spdlog::level::trace); 
    std::string inputFile;
    double deltaT;
    double endTime;
    std::unique_ptr<outputWriters::OutputWriter> outputWriter;
    std::unique_ptr<Calculators::Calculator> calculator;

    if (!MolSim::parseArguments(argc, argsv, inputFile, deltaT, endTime, outputWriter, calculator)) {
        return 1;
    }

    ParticleContainer particleContainer;

    FileReader fileReader;
    fileReader.readFile(particleContainer, inputFile);
    particleContainer.initializePairsVector();

    SPDLOG_INFO("Hello from MolSim for PSE!");
    SPDLOG_INFO("Simulation starting! deltaT = {}, endTime = {}", deltaT, endTime);

    MolSim::runSim(particleContainer, deltaT, endTime, outputWriter, calculator);

    SPDLOG_DEBUG("Simulation finished!");

    return 0;
}
