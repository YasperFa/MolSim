#include "CheckpointFileReader.h"
#include <fstream>
#include "IO/Input/CheckpointInput/CheckpointSchema.h"
#include "spdlog/spdlog.h"
#include <memory>

void CheckpointFileReader::printSimulationData(const SimulationDataType &simulationData) {
    SPDLOG_INFO("Checkpoint loaded with the following data:");
    SPDLOG_INFO("-> Input file: {}", simulationData.inputFile());
    SPDLOG_INFO("-> endTime: {}", simulationData.endTime());
    SPDLOG_INFO("-> deltaT: {}", simulationData.deltaT());
    SPDLOG_INFO("-> gravity: {}", simulationData.gravity());
}

void CheckpointFileReader::readCheckpoint(std::string &filePath,
                                          std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer) {
    try {
        // Open the checkpoint file
        std::ifstream fileStream(filePath);
        if (!fileStream.is_open()) {
            SPDLOG_ERROR("Could not open the checkpoint file: {}", filePath);
            throw std::runtime_error("Failed to open checkpoint file");
        }

        // Parse the checkpoint file
        SPDLOG_INFO("Parsing checkpoint file: {}", filePath);
        auto checkpoint = Checkpoint(fileStream, xml_schema::flags::dont_validate);

        // Extract simulation data
        const auto &simulationData = checkpoint->SimulationData();

        CheckpointFileReader::printSimulationData(simulationData);

        // Extract particle data
        const auto &particleData = checkpoint->ParticleData();
        for (const auto &particle: particleData.particle()) {
            std::array<double, 3> position = {
                particle.position().x(), particle.position().y(), particle.position().z()
            };
            std::array<double, 3> velocity = {
                particle.velocity().x(), particle.velocity().y(), particle.velocity().z()
            };
            std::array<double, 3> force = {particle.force().x(), particle.force().y(), particle.force().z()};
            std::array<double, 3> oldForce = {
                particle.oldForce().x(), particle.oldForce().y(), particle.oldForce().z()
            };
            double mass = particle.mass();
            int type = particle.type().present() ? particle.type().get() : 0;

            Particle newParticle(position, velocity, mass, type);
            newParticle.setF(force);
            newParticle.setOldF(oldForce);
            particleContainer->addParticle(newParticle);
        }

        SPDLOG_INFO("Checkpoint successfully parsed and data loaded.");
    } catch (const xml_schema::exception &e) {
        SPDLOG_ERROR("XML schema validation error while reading checkpoint: {}", e.what());
        throw;
    } catch (const std::exception &e) {
        SPDLOG_ERROR("Error reading checkpoint file: {}", e.what());
        throw;
    }
}
