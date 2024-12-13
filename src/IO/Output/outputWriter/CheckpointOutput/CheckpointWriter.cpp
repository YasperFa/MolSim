//
// Created by Celik Yucel on 10.12.24.
//

#include "CheckpointWriter.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "spdlog/spdlog.h"

namespace outputWriters {
    void CheckpointWriter::plotParticles(int iteration, ParticleContainers::ParticleContainer &particleContainer,
        const std::string &filename, const std::string input, double &endTime, double &gravity, double &deltaT) {

        std::stringstream strstr;
        strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration << ".chk";

        std::ofstream chkFile(strstr.str());

        if (!chkFile.is_open()) {
            SPDLOG_ERROR("Could not open the checkpoint file to write");
            throw std::runtime_error("Could not open the checkpoint file");
        }

        chkFile << R"(<?xml version="1.0" encoding="UTF-8"?>)" << "\n";
        chkFile << "<Checkpoint>\n";

        chkFile << "<SimulationData>\n";
        chkFile << "    <inputFile>" << input << "</inputFile>\n";
        chkFile << "    <endTime>" << endTime << "</endTime>\n";
        chkFile << "    <deltaT>" << deltaT << "</deltaT>\n";
        chkFile << "    <gravity>" << gravity << "</gravity>\n";
        chkFile << "</SimulationData>\n";

        chkFile << "<ParticleData>\n";
        for (auto& particle : particleContainer) {
            chkFile << "    <particle>\n";

            chkFile << "        <position>\n";
            chkFile << "            <x>" << particle.getX()[0] << "</x>\n";
            chkFile << "            <y>" << particle.getX()[1] << "</y>\n";
            chkFile << "            <z>" << particle.getX()[2] << "</z>\n";
            chkFile << "        </position>\n";

            chkFile << "        <velocity>\n";
            chkFile << "            <x>" << particle.getV()[0] << "</x>\n";
            chkFile << "            <y>" << particle.getV()[1] << "</y>\n";
            chkFile << "            <z>" << particle.getV()[2] << "</z>\n";
            chkFile << "        </velocity>\n";

            chkFile << "        <mass>" << particle.getM() << "</mass>\n";

            chkFile << "        <type>" << particle.getType() << "</type>\n";

            chkFile << "        <force>\n";
            chkFile << "            <x>" << particle.getF()[0] << "</x>\n";
            chkFile << "            <y>" << particle.getF()[1] << "</y>\n";
            chkFile << "            <z>" << particle.getF()[2] << "</z>\n";
            chkFile << "        </force>\n";

            chkFile << "        <oldForce>\n";
            chkFile << "            <x>" << particle.getOldF()[0] << "</x>\n";
            chkFile << "            <y>" << particle.getOldF()[1] << "</y>\n";
            chkFile << "            <z>" << particle.getOldF()[2] << "</z>\n";
            chkFile << "        </oldForce>\n";
            chkFile << "    </particle>\n";

        }
        chkFile << "</ParticleData>\n";

        chkFile << "</Checkpoint>\n";

        chkFile.close();
    }

    std::string CheckpointWriter::toString() {
        return {"CheckpointWriter"};
    }


}
