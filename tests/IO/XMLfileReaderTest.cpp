//
// Created by Yasmine Farah on 24/11/2024.
//
#include <gtest/gtest.h>
#include "../../src/IO/Input/XMLfileReader.h"

// check if parsing of the xml file MolSim/input/schemaTest1.xml
TEST(XMLfileReaderTest, correctParsingSchemaTest1){
    std::ifstream file("../input/schemaTest1.xml");
    double deltaT;
    double endTime;
    double gravity;
    int freq;
    bool version2 = false;
    std::unique_ptr<outputWriters::OutputWriter> outputWriter;
    std::unique_ptr<Calculators::Calculator> calculator;
    std::unique_ptr<ParticleContainers::ParticleContainer> particleContainer;
    std::unique_ptr<BoundaryHandler> boundaryHandler;
    std::unique_ptr<Thermostat> thermostat;
    XMLfileReader::parseXMLFromFile(file, deltaT, endTime, gravity, freq,version2,outputWriter, calculator, particleContainer, boundaryHandler,thermostat);
    EXPECT_EQ("XYZWriter",outputWriter->toString());
    EXPECT_EQ("LJC",calculator->toString());
    EXPECT_EQ(10, freq);
    EXPECT_NEAR(0.0001, deltaT, 0.00001);
    EXPECT_NEAR(3.0, endTime, 0.00001);
    for (Particle p : *particleContainer) {
        // check for correct position
        EXPECT_NEAR(1.0, p.getX()[0], 0.00001);
        EXPECT_NEAR(2.0, p.getX()[1], 0.00001);
        EXPECT_NEAR(3.0, p.getX()[2], 0.00001);
        //check for correct V
        EXPECT_NEAR(0.0, p.getV()[0], 0.00001);
        EXPECT_NEAR(1.0, p.getV()[1], 0.00001);
        EXPECT_NEAR(0.0, p.getV()[2], 0.00001);
        //check for correct mass
        EXPECT_NEAR(5.0, p.getM(), 0.00001);
    }

}
// check if parsing of the xml file MolSim/input/schemaTest2.xml
TEST(XMLfileReaderTest, correctParsingSchemaTest2) {
    std::ifstream file("../input/schemaTest2.xml");
    double deltaT = 0.0;
    double endTime = 0.0;
    double gravity = 0;
    bool version2 = false;
    int freq;
    std::unique_ptr<outputWriters::OutputWriter> outputWriter;
    std::unique_ptr<Calculators::Calculator> calculator;
    std::unique_ptr<ParticleContainers::ParticleContainer> particleContainer;
    std::unique_ptr<BoundaryHandler> boundaryHandler;
    std::unique_ptr<Thermostat> thermostat;
    XMLfileReader::parseXMLFromFile(file, deltaT, endTime, gravity, freq,version2,outputWriter, calculator, particleContainer, boundaryHandler,thermostat);
    EXPECT_EQ("VTKWriter",outputWriter->toString());
   EXPECT_EQ("Default",calculator->toString());
    EXPECT_EQ(1, freq);
    SPDLOG_INFO("{}", deltaT);
    EXPECT_NEAR(0.0, deltaT, 0.00001);
    EXPECT_NEAR(0.0, endTime, 0.00001);
}