//
// Created by Yasmine Farah on 16/11/2024.
//

#include "XMLfileReader.h"
#include <fstream>
#include <iostream>
#include <xsd/cxx/xml/dom/parsing-source.hxx>
#include "schema.h"
#include "spdlog/spdlog.h"
#include <float.h>

#include "IO/Output/outputWriter/CheckpointOutput/CheckpointWriter.h"


int XMLfileReader::parseXMLFromFile(std::ifstream &fileStream, double &deltaT, double &endTime, double &gravity,
                                    int &freq, bool &version2,
                                    std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                                    std::unique_ptr<Calculators::Calculator> &calculator,
                                    std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer,
                                    std::unique_ptr<BoundaryHandler> &boundaryHandler,
                                    std::unique_ptr<Thermostat> &thermostat) {
    if (!fileStream) {
        SPDLOG_ERROR("Error: Unable to open file");
        return 1;
    }
    try {
        // Parse the XML file into a DOM object
        std::unique_ptr<simulation> sim = simulation_(fileStream);

        // Access the parsed data
        if (sim.get() != nullptr) {
            // Check if parsing succeeded

            if (sim->output().writeFrequency().present()) {
                freq = sim->output().writeFrequency().get();
                SPDLOG_DEBUG("Writer frequency from xml: {}", freq);
                if (freq <= 0) {
                    SPDLOG_ERROR("Invalid Frequency, Frequency should be positive!, using default value");
                    freq = 10;
                }
            }
            if (sim->parameters().parallelVersion().present()) {
                version2 = sim->parameters().parallelVersion().get();
                SPDLOG_INFO("Parallel version from XML selected: {}", version2);
            }
            SPDLOG_DEBUG("Reading container from file");
            if (sim->container().containerType() == "DSC") {
                SPDLOG_DEBUG("DSC selected from xml file");
                particleContainer = std::make_unique<ParticleContainers::DirectSumContainer>();
            } else if (sim->container().containerType() == "LCC") {
                SPDLOG_DEBUG("LCC selected from xml file");
                std::array<double, 3> domainSizeArray = {180.0, 90.0, 1.0};
                double cutoffRadius = 3.0;
                if (sim->container().cutoffRadius().present()) {
                    cutoffRadius = sim->container().cutoffRadius().get();
                }
                if (sim->container().domainSize().present()) {
                    domainSizeArray[0] = sim->container().domainSize().get().x();
                    domainSizeArray[1] = sim->container().domainSize().get().y();
                    domainSizeArray[2] = sim->container().domainSize().get().z();
                }
                particleContainer = std::make_unique<ParticleContainers::LinkedCellContainer>(domainSizeArray, cutoffRadius, version2);
                if(sim -> container().boundaryType().present()) {
                   std::array<BoundaryHandler::bCondition, 6> condition;
                    condition[0] = getConditionType(sim ->container().boundaryType().get().left());
                    condition[1] = getConditionType(sim ->container().boundaryType().get().right());
                    condition[2] = getConditionType(sim ->container().boundaryType().get().top());
                    condition[3] = getConditionType(sim ->container().boundaryType().get().bottom());
                    condition[4] = getConditionType(sim ->container().boundaryType().get().front());
                    condition[5] = getConditionType(sim ->container().boundaryType().get().back());
                    boundaryHandler = std::make_unique<BoundaryHandler>(condition , *(dynamic_cast <ParticleContainers::LinkedCellContainer*>(&(*particleContainer))));
                }

            }
            else {
                SPDLOG_ERROR("Invalid container type! choose one of the following: DSC / LCC");
            }
            if (sim->parameters().deltaT().present()) {
                deltaT = sim->parameters().deltaT().get();
                SPDLOG_DEBUG("deltaT from XML selected: {}", deltaT);
                if (deltaT <= 0) {
                    SPDLOG_ERROR("Invalid deltaT, deltaT should be positive!, using default value");
                    deltaT = 0.0002;
                }
            }
            if (sim->parameters().tEnd().present()) {
                endTime = sim->parameters().tEnd().get();
                SPDLOG_DEBUG("endTime from XML selected: {}", endTime);
                if (endTime < 0) {
                    SPDLOG_ERROR("Invalid endTime, endTime should be >= 0 !, using default value");
                    endTime = 5;
                }
            }
            if (sim->parameters().gravity().present()) {
                gravity = sim->parameters().gravity().get();
                SPDLOG_DEBUG("gravity from XML selected: {}", gravity);
            }

            if (sim->output().baseName().present()) {
                std::string output = sim->output().baseName().get();
                if (output == "VTK") {
                    SPDLOG_DEBUG("vtk is selected from xml as the output writer");
                    outputWriter = std::make_unique<outputWriters::VTKWriter>();
                } else if (output == "XYZ") {
                    outputWriter = std::make_unique<outputWriters::XYZWriter>();
                    SPDLOG_DEBUG("xyz is selected from xml as the output writer");
                } else if (output == "CHK") {
                    outputWriter = std::make_unique<outputWriters::CheckpointWriter>();
                    SPDLOG_DEBUG("chekcpoint is selected from xml as the output writer");
                } else {
                    SPDLOG_ERROR(
                        "Erroneous programme call! Invalid output writer specified! Output should be XYZ / VTK");
                    return 1;
                }
            }
            if (sim->calculator().calculatorForce().present()) {
                std::string calculatorType = sim->calculator().calculatorForce().get();
                if (calculatorType == "LJC") {
                    calculator = std::make_unique<Calculators::LennardJonesCalculator>();
                    SPDLOG_DEBUG("LJC is selected from xml");
                } else if (calculatorType == "Default") {
                    calculator = std::make_unique<Calculators::GravityCalculator>();
                    SPDLOG_DEBUG("Default calculator is selected from xml");
                } else {
                    SPDLOG_ERROR(
                        "Erroneous programme call! Invalid calculator specified! calculator should be Default / LJC.");
                    return 1;
                }
            }
            //use it later for initial velocities (see shapes parsing)
            double initialTemperature = -1;
            if (sim->temperature().present()) {
                initialTemperature = sim->temperature().get().initialTemperature();
                int timeSteps = sim->temperature().get().timeSteps();
                double targetTemperature = initialTemperature;
                double maxDeltaT = DBL_MAX;
                if (sim->temperature().get().targetTemperature().present()) {
                    targetTemperature = sim->temperature().get().targetTemperature().get();
                }
                if (sim->temperature().get().maxDeltaTemperature().present()) {
                    maxDeltaT = sim->temperature().get().maxDeltaTemperature().get();
                }
                thermostat = std::make_unique<DirectThermostat>(targetTemperature, maxDeltaT, initialTemperature, timeSteps);
                SPDLOG_DEBUG("Direct thermostat is selected from xml");

            }
            for (int i = 0; i < (int) sim->shapes().particle().size(); i++) {
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

                int type = 0;
                if (sim->shapes().particle().at(i).type().present()) {
                    type = sim->shapes().particle().at(i).type().get();
                }

                double epsilon = 5;
                if (sim->shapes().particle().at(i).epsilon().present()) {
                    epsilon = sim->shapes().particle().at(i).epsilon().get();
                }

                double sigma = 1;
                if (sim->shapes().particle().at(i).sigma().present()) {
                    sigma = sim->shapes().particle().at(i).sigma().get();
                }

                Particle newParticle(x, v, m, type, epsilon, sigma);
                (particleContainer)->addParticle(newParticle);
            }
            for (int i = 0; i < (int) sim->shapes().cuboid().size(); i++) {
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


                int type = 0;
                if (sim->shapes().cuboid().at(i).type().present()) {
                    type = sim->shapes().cuboid().at(i).type().get();
                }

                double epsilon = 5;
                if (sim->shapes().cuboid().at(i).epsilon().present()) {
                    epsilon = sim->shapes().cuboid().at(i).epsilon().get();
                }

                double sigma = 1;
                if (sim->shapes().cuboid().at(i).sigma().present()) {
                    sigma = sim->shapes().cuboid().at(i).sigma().get();
                }


                Cuboid cuboid(x,N,h,m,v,mv);
                ParticleGenerator::generateCuboid(*particleContainer, cuboid, type, epsilon, sigma, initialTemperature);
            }
            for (int i = 0; i < (int) sim->shapes().disc().size(); i++) {
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

                int type = 0;
                if (sim->shapes().disc().at(i).type().present()) {
                    type = sim->shapes().disc().at(i).type().get();
                }

                double epsilon = 5;
                if (sim->shapes().disc().at(i).epsilon().present()) {
                    epsilon = sim->shapes().disc().at(i).epsilon().get();
                }

                double sigma = 1;
                if (sim->shapes().disc().at(i).sigma().present()) {
                    sigma = sim->shapes().disc().at(i).sigma().get();
                }


                Disc disc(x, v, radius, h, m);
                ParticleGenerator::generateDisc(*particleContainer, disc, type, epsilon, sigma);
            }


            return 0;
        } else {
            SPDLOG_DEBUG("Parsing returned a null object.");
            return 1;
        }
    } catch
    (
        const xml_schema::exception &e
    ) {
        // Handle parsing exceptions
        SPDLOG_ERROR("XML parsing error: {}, invalid xml format", e.what());
        return 1;
    }
}

BoundaryHandler::bCondition XMLfileReader::getConditionType(std::string input){
    if (input == "reflecting"){
        return BoundaryHandler::bCondition::REFLECTING;
    } else if (input == "periodic"){
        return BoundaryHandler::bCondition::PERIODIC;
    } else return BoundaryHandler::bCondition::OUTFLOW; //xs enumeration prevents other values
};
