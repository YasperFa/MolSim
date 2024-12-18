#include <iostream>
#include <list>
#include <functional>

#include "MolSimFunctions.h"

#include "IO/Input/XMLfileReader.h"
#include "IO/Input/CheckpointInput/CheckpointReader/CheckpointFileReader.h"


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
    int freq = 20;
    double gravity;
    std::unique_ptr<outputWriters::OutputWriter> outputWriter;
    std::unique_ptr<Calculators::Calculator> calculator;
    std::unique_ptr<ParticleContainers::ParticleContainer> particleContainer;
    std::unique_ptr<BoundaryHandler> boundaryHandler;
    std::unique_ptr<Thermostat> thermostat;
    //parses and sets arguments from the command line
    if (!MolSim::parseArguments(argc, argsv, inputFile, deltaT, endTime, gravity,  outputWriter, calculator, particleContainer, boundaryHandler)) {
        return 1;
       }

    //if the specified input file is xml, overwrite and set new arguments
    if (inputFile.compare(inputFile.length() - 4, 4, ".xml") == 0) {
        std::ifstream file(inputFile);
        if (XMLfileReader::parseXMLFromFile(file,deltaT,endTime, gravity, freq, outputWriter, calculator, particleContainer, boundaryHandler, thermostat)) {
            return 1;
        }

        if (MolSim::runSubSim(inputFile)) {
            SPDLOG_INFO("Subsimulations completed successfully!");
            MolSim::loadCheckpoints(particleContainer);
        }

    } else if (inputFile.compare(inputFile.length() - 4, 4, ".chk") == 0) {
        CheckpointFileReader::readCheckpoint(inputFile, particleContainer);
    } else{
        FileReader fileReader;
        fileReader.readFile(*particleContainer, inputFile);
    }




    SPDLOG_INFO("Hello from MolSim for PSE!");
    SPDLOG_INFO("Simulation starting! deltaT = {}, endTime = {}", deltaT, endTime);

    MolSim::runSim(*particleContainer, deltaT, endTime, gravity, freq, outputWriter, calculator, boundaryHandler, thermostat, inputFile);

    SPDLOG_DEBUG("Simulation finished!");

    return 0;

}
