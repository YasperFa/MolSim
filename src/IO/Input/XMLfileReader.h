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
#include "Objects/Cuboid.h"
#include "Objects/ParticleGenerator.h"
#include <memory>
class XMLfileReader {
   public:
    /**Parses data from an xml file
   * @param fileStream: stream of the xml file
   * @param deltaT: delta_t
   * @param endTime: end_time
   * @param freq: frequency of output files
   * @param outputWriter: outputWriter vtk/xyz
   * @param calculator: calculator LJC/Default
   * @param particleContainer: particle container
   */
   static int parseXMLFromFile(std::ifstream& fileStream,double &deltaT, double &endTime, int &freq,
                               std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                               std::unique_ptr<Calculators::Calculator> &calculator, std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer, std::unique_ptr<BoundaryHandler> &boundaryHandler);
};


