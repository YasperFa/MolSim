//
// Created by Yasmine Farah on 16/11/2024.
//

#include "XMLfileReader.h"
#include <fstream>
#include <iostream>
#include <xsd/cxx/xml/dom/parsing-source.hxx>
#include "schema.h"
#include "spdlog/spdlog.h"



int XMLfileReader::parseXMLFromFile(std::ifstream& fileStream,double &deltaT, double &endTime, int &freq,
                                     std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                                     std::unique_ptr<Calculators::Calculator> &calculator, std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer,  std::unique_ptr<BoundaryHandler> &boundaryHandler) {

    if (!fileStream) {
        SPDLOG_ERROR("Error: Unable to open file");
        return 1;
    }
    try {
        // Parse the XML file into a DOM object
        std::unique_ptr<simulation> sim = simulation_(fileStream);

        // Access the parsed data
        if (sim.get() != nullptr) { // Check if parsing succeeded

            if(sim->output().writeFrequency().present()){
                freq = sim->output().writeFrequency().get();
                SPDLOG_DEBUG("Writer frequency from xml: {}", freq);
                if (freq <= 0){
                    SPDLOG_ERROR("Invalid Frequency, Frequency should be positive!, using default value");
                    freq = 10;
                }
            }
            SPDLOG_DEBUG("Reading container from file");
            if(sim-> container().containerType() == "DSC") {
                SPDLOG_DEBUG("DSC selected from xml file");
                particleContainer = std::make_unique<ParticleContainers::DirectSumContainer>();
            } else if(sim-> container().containerType() == "LCC") {
                SPDLOG_DEBUG("LCC selected from xml file");
                std::array<double,3> domainSizeArray = {180.0,90.0,1.0};
                double cutoffRadius = 3.0;
                if(sim-> container().cutoffRadius().present()) {
                    cutoffRadius = sim-> container().cutoffRadius().get();
                }
                if(sim ->container().domainSize().present()) {
                    domainSizeArray[0] = sim->container().domainSize().get().x();
                    domainSizeArray[1] = sim->container().domainSize().get().y();
                    domainSizeArray[2] = sim->container().domainSize().get().z();
                }
                if(sim -> container().BoundaryType().present()) {
                    std::string condition = sim ->container().BoundaryType().get();
                    if(condition == "outflow") {
                        boundaryHandler = std::make_unique<BoundaryHandler>(1, 0, *(dynamic_cast <ParticleContainers::LinkedCellContainer*>(&(*particleContainer)))); //sigma is hardcoded for now
                    } else if (condition == "reflecting"){
                        boundaryHandler = std::make_unique<BoundaryHandler>(1, 1, *(dynamic_cast <ParticleContainers::LinkedCellContainer*>(&(*particleContainer))));
                    } else {
                        SPDLOG_ERROR("Boundary condition is not set correctly");
                        return false;
                    }
                }
                particleContainer = std::make_unique<ParticleContainers::LinkedCellContainer>(domainSizeArray, cutoffRadius);
            }
            else {
                SPDLOG_ERROR("Invalid container type!");
            }
            if(sim->parameters().deltaT().present()){

                deltaT = sim->parameters().deltaT().get();
                SPDLOG_DEBUG("deltaT from XML selected: {}",deltaT);
                if (deltaT <= 0){
                    SPDLOG_ERROR("Invalid deltaT, deltaT should be positive!, using default value");
                    deltaT = 0.0002;
                }
            }
            if(sim->parameters().tEnd().present()){
                endTime = sim->parameters().tEnd().get();
                SPDLOG_DEBUG("endTime from XML selected: {}", endTime);
                if (endTime < 0){
                    SPDLOG_ERROR("Invalid endTime, endTime should be >= 0 !, using default value");
                    endTime = 5;
                }
            }
            if (sim->output().baseName().present())
            {
                std::string output = sim->output().baseName().get();
                if(output == "VTK")
                {
                    SPDLOG_DEBUG("vtk is selected from xml as the output writer");
                    outputWriter = std::make_unique<outputWriters::VTKWriter>();
                }
                else if(output == "XYZ")
                {
                    outputWriter = std::make_unique<outputWriters::XYZWriter>();
                    SPDLOG_DEBUG("xyz is selected from xml as the output writer");
                } else {
                    SPDLOG_ERROR("Erroneous programme call! Invalid output writer specified! Using default value/value from flag");
                    return 1;
                }
            }
            if(sim->calculator().calculatorForce().present())
            {
                std::string calculatorType = sim->calculator().calculatorForce().get();
                if (calculatorType == "LJC")
                {
                    calculator = std::make_unique<Calculators::LennardJonesCalculator>();
                    SPDLOG_DEBUG("LJC is selected from xml");
                }
                else if (calculatorType == "Default")
                {
                    calculator = std::make_unique<Calculators::GravityCalculator>();
                    SPDLOG_DEBUG("Default calculator is selected from xml");
                } else {
                    SPDLOG_ERROR("Erroneous programme call! Invalid calculator specified! Using default value/value from flag");
                    return 1;
                }
            }
            for (int i=0; i < (int) sim->shapes().particle().size(); i++) {
                SPDLOG_DEBUG("reading particles from xml file");
                // define all particle parameters
                std::array<double, 3> x;
                x[0] = sim->shapes().particle().at(i).position().x();
                x[1] = sim->shapes().particle().at(i).position().y();
                x[2] = sim->shapes().particle().at(i).position().z();
                std::array<double, 3> v;
                v[0] = sim->shapes().particle().at(i).velocity().x();
                v[1] = sim->shapes().particle().at(i).velocity().y();
                v[2] = sim->shapes().particle().at(i).velocity().z();
                double m = sim->shapes().particle().at(i).mass();
                Particle newParticle(x,v,m,0);
                (particleContainer) -> addParticle(newParticle);
            }
            for (int i=0; i < (int) sim->shapes().cuboid().size();i++){
                SPDLOG_DEBUG("reading cuboids from xml file");
                // define all Cuboid parameters
                std::array<double, 3> x;
                x[0] = sim->shapes().cuboid().at(i).position().x();
                x[1] = sim->shapes().cuboid().at(i).position().y();
                x[2] = sim->shapes().cuboid().at(i).position().z();
                std::array<double, 3> N;
                N[0] = sim->shapes().cuboid().at(i).dimensions().x();
                N[1] = sim->shapes().cuboid().at(i).dimensions().y();
                N[2] = sim->shapes().cuboid().at(i).dimensions().z();
                std::array<double, 3> v;
                v[0] = sim->shapes().cuboid().at(i).initialVelocity().x();
                v[1] = sim->shapes().cuboid().at(i).initialVelocity().y();
                v[2] = sim->shapes().cuboid().at(i).initialVelocity().z();
                double h = sim->shapes().cuboid().at(i).distance();
                double m = sim->shapes().cuboid().at(i).mass();
                double mv = sim->shapes().cuboid().at(i).meanVelocity();
                Cuboid cuboid(x,N,h,m,v,mv);
                ParticleGenerator::generateCuboid(*particleContainer, cuboid);
            }
            for (int i=0; i < (int) sim->shapes().disc().size();i++){
                SPDLOG_DEBUG("reading discs from xml file");
                // define all disc parameters
                std::array<double, 3> x;
                x[0] = sim->shapes().disc().at(i).center().x();
                x[1] = sim->shapes().disc().at(i).center().y();
                x[2] = sim->shapes().disc().at(i).center().z();
                std::array<double, 3> v;
                v[0] = sim->shapes().disc().at(i).initialVelocity().x();
                v[1] = sim->shapes().disc().at(i).initialVelocity().y();
                v[2] = sim->shapes().disc().at(i).initialVelocity().z();
                double h = sim->shapes().disc().at(i).distance();
                double m = sim->shapes().disc().at(i).mass();
                double radius = sim->shapes().disc().at(i).radius();

                Disc disc(x,v,radius,h,m);
                ParticleGenerator::generateDisc(*particleContainer, disc);
            }


        return 0;
        } else {
            SPDLOG_DEBUG("Parsing returned a null object.");
            return 1;
        }
    } catch (const xml_schema::exception& e) {
        // Handle parsing exceptions
        SPDLOG_ERROR("XML parsing error: {}, invalid xml format" , e.what());
        return 1;
    }
}
