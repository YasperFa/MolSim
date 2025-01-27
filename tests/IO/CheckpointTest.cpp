#include <gtest/gtest.h>
#include <fstream>
#include <memory>
#include "../../src/IO/Input/CheckpointInput/CheckpointReader/CheckpointFileReader.h"
#include "../../src/IO/Output/outputWriter/CheckpointOutput/CheckpointWriter.h"

#include "../../src/Objects/Containers/DirectSum/DirectSumContainer.h"

TEST(CheckpointFileReaderTest, ReadCheckpointFileSuccessfully) {
    // Prepare a checkpoint file
    std::string filePath = "test_checkpoint.chk";  // Make this non-const
    std::ofstream testFile(filePath);
    testFile << R"(<?xml version="1.0" encoding="UTF-8"?>)" << "\n";
    testFile << "<Checkpoint>\n";
    testFile << "  <SimulationData>\n";
    testFile << "      <inputFile>input.txt</inputFile>\n";
    testFile << "      <endTime>100.0</endTime>\n";
    testFile << "      <deltaT>0.01</deltaT>\n";
    testFile << "      <gravity>9.8</gravity>\n";
    testFile << "  </SimulationData>\n";
    testFile << "  <ParticleData>\n";
    testFile << "      <particle>\n";
    testFile << "          <position><x>1.0</x><y>2.0</y><z>3.0</z></position>\n";
    testFile << "          <velocity><x>0.1</x><y>0.2</y><z>0.3</z></velocity>\n";
    testFile << "          <mass>1.5</mass>\n";
    testFile << "          <type>1</type>\n";
    testFile << "          <force><x>0.0</x><y>0.0</y><z>0.0</z></force>\n";
    testFile << "          <oldForce><x>0.1</x><y>0.1</y><z>0.1</z></oldForce>\n";
    testFile << "      </particle>\n";
    testFile << "  </ParticleData>\n";
    testFile << "</Checkpoint>\n";
    testFile.close();

    // Test reading the checkpoint
    std::unique_ptr<ParticleContainers::ParticleContainer> particleContainer = std::make_unique<ParticleContainers::DirectSumContainer>();
    CheckpointFileReader reader;

    ASSERT_NO_THROW(reader.readCheckpoint(filePath, particleContainer));
    EXPECT_EQ(particleContainer->sizeParticles(), 1);

    Particle p = particleContainer->getParticles()[0];;
    EXPECT_EQ(p.getX()[0], 1.0);
    EXPECT_EQ(p.getX()[1], 2.0);
    EXPECT_EQ(p.getX()[2], 3.0);
    EXPECT_EQ(p.getV()[0], 0.1);
    EXPECT_EQ(p.getV()[1], 0.2);
    EXPECT_EQ(p.getV()[2], 0.3);
    EXPECT_EQ(p.getM(), 1.5);
    EXPECT_EQ(p.getType(), 1);

    // Clean up
    std::remove(filePath.c_str());
}

TEST(CheckpointWriterTest, WriteCheckpointFileSuccessfully) {
    ParticleContainers::DirectSumContainer particleContainer;
    Particle p({1.0, 2.0, 3.0}, {0.1, 0.2, 0.3}, 1.5, 1);
    p.setF({0.0, 0.0, 0.0});
    p.setOldF({0.1, 0.1, 0.1});
    particleContainer.addParticle(p);

    const std::string filename = "test_checkpoint";
    const std::string input = "input.txt";
    double endTime = 100;
    double gravity = 9.8;
    double deltaT = 0.01;

    outputWriters::CheckpointWriter writer;
    ASSERT_NO_THROW(writer.plotParticles(1, particleContainer, filename, input, endTime, gravity, deltaT));

    // Verify the file contents
    std::ifstream chkFile("test_checkpoint_0001.chk");
    ASSERT_TRUE(chkFile.is_open());

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(chkFile, line)) {
        lines.push_back(line);
    }
    chkFile.close();

    // Check basic file structure
    ASSERT_EQ(lines[0], R"(<?xml version="1.0" encoding="UTF-8"?>)");
    ASSERT_EQ(lines[1], "<Checkpoint>");
    ASSERT_EQ(lines[2], "<SimulationData>");
    ASSERT_EQ(lines[3], "    <inputFile>input.txt</inputFile>");
    ASSERT_EQ(lines[4], "    <endTime>100</endTime>");
    ASSERT_EQ(lines[5], "    <deltaT>0.01</deltaT>");
    ASSERT_EQ(lines[6], "    <gravity>9.8</gravity>");
    ASSERT_EQ(lines[7], "</SimulationData>");
    ASSERT_EQ(lines[8], "<ParticleData>");
    ASSERT_EQ(lines[9], "    <particle>");
    ASSERT_EQ(lines[10], "        <position>");
    ASSERT_EQ(lines[11], "            <x>1</x>");
    ASSERT_EQ(lines[12], "            <y>2</y>");
    ASSERT_EQ(lines[13], "            <z>3</z>");
    ASSERT_EQ(lines[32], "    </particle>");
    ASSERT_EQ(lines.back(), "</Checkpoint>");

    // Clean up
    std::remove("test_checkpoint_0001.chk");
}

TEST(CheckpointWriterTest, WriteCheckpointFileFailure) {
    ParticleContainers::DirectSumContainer particleContainer;
    Particle p({1.0, 2.0, 3.0}, {0.1, 0.2, 0.3}, 1.5, 1);
    p.setF({0.0, 0.0, 0.0});
    p.setOldF({0.1, 0.1, 0.1});
    particleContainer.addParticle(p);

    const std::string filename = "/invalid_path/test_checkpoint";
    const std::string input = "input.txt";
    double endTime = 100.0;
    double gravity = 9.8;
    double deltaT = 0.01;

    outputWriters::CheckpointWriter writer;
    ASSERT_THROW(writer.plotParticles(1, particleContainer, filename, input, endTime, gravity, deltaT), std::runtime_error);
}