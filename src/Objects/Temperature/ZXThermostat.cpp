//
// Created by Sawsan Farah on 18/01/2025.
//

#include "ZXThermostat.h"

void ZXThermostat::applyThermostat(ParticleContainers::ParticleContainer& particleContainer) {
    double temperatureChange;
    std::array<double, 3> averageVelocity = getAverageVelocity(particleContainer);
    double keneticEnergy = 0;
    for (auto& particle : particleContainer) {
        if (particle.getFixed()) continue;
        std::array<double, 3> v = particle.getV();
        keneticEnergy += particle.getM() * (v[0] * v[0] + (v[1] - averageVelocity[1])*(v[1] - averageVelocity[1])  + v[2] * v[2]) * 0.5;
    }
    int dimension = is3D ? 3 : 2;
    const double currentTemperature = 2 * keneticEnergy / (dimension * particleContainer.sizeParticles());
    if (targetTemperature > currentTemperature) {
        temperatureChange = std::min(targetTemperature - currentTemperature, maxDeltaT);
    } else {
        temperatureChange = std::max(targetTemperature - currentTemperature, -maxDeltaT);
    }
    const double newTemperature = currentTemperature + temperatureChange;
    const double scaleFactor = std::sqrt(newTemperature / currentTemperature);
    for (auto &particle: particleContainer) {
        if (particle.getFixed()) continue;
        std::array<double, 3> newV;
         newV[0] = scaleFactor*particle.getV()[0];
         newV[1] = particle.getV()[1];
         newV[2] = scaleFactor*particle.getV()[2];
        particle.setV(newV);
    }
}
