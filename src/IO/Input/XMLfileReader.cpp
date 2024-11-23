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
                                     std::unique_ptr<Calculators::Calculator> &calculator, ParticleContainer &particleContainer) {

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
                    SPDLOG_DEBUG("Default calculator is selected from xml");
                    calculator = std::make_unique<Calculators::Calculator>();
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
                particleContainer.addParticle(newParticle);
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
                ParticleGenerator::generateCuboid(particleContainer, cuboid);
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
                ParticleGenerator::generateDisc(particleContainer, disc);
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