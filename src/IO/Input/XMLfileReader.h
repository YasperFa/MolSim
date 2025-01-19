//
// Created by Yasmine Farah on 16/11/2024.
//

#pragma once
#include <string>
#include "spdlog/spdlog.h"
#include "Calculator/Calculator.h"
#include "IO/Output/outputWriter/OutputWriter.h"
#include "IO/Output/outputWriter/VTKWriter.h"
#include "IO/Output/outputWriter/XYZWriter.h"
#include "Calculator/Calculator.h"
#include "Calculator/LennardJonesCalculator.h"
#include "Calculator/GravityCalculator.h"
#include "Objects/Shapes/Cuboid.h"
#include "Objects/ParticleGenerator.h"
#include "../../Objects/Temperature/Thermostat.h"
#include "../../Objects/Temperature/DirectThermostat.h"
#include <memory>

#include "Objects/Temperature/Thermostat.h"

/**Class for parsing xml input from a file */
class XMLfileReader {
   private:

   static BoundaryHandler::bCondition getConditionType(std::string input);

   public:
    /**Parses data from an xml file
   * @param fileStream: stream of the xml file
   * @param deltaT: delta_t
   * @param endTime: end_time
   * @param freq: frequency of output files
   * @param outputWriter: outputWriter vtk/xyz/chk
   * @param calculator: calculator LJC/Default
   * @param particleContainer: particle container
   * @param boundaryHandler: boundary handler
   * @param thermostat: thermostat
   */
   static int parseXMLFromFile(std::ifstream& fileStream,double &deltaT, double &endTime, double& gravity, int &freq,
                               std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                               std::unique_ptr<Calculators::Calculator> &calculator, std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer, std::unique_ptr<BoundaryHandler> &boundaryHandler,
                               std::unique_ptr<Thermostat> &thermostat);
};


