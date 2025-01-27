//
// Created by Yasmine Farah on 05/12/2024.
//

#include "DirectThermostat.h"
#include "utils/ArrayUtils.h"


void DirectThermostat::applyThermostat(ParticleContainers::ParticleContainer& particleContainer) {
    const double currentTemperature = getCurrentTemperature(particleContainer, is3D);
    double temperatureChange;
    if (targetTemperature > currentTemperature) {
         temperatureChange = std::min(targetTemperature - currentTemperature, maxDeltaT);
    } else {
         temperatureChange = std::max(targetTemperature - currentTemperature, -maxDeltaT);
    }
    const double newTemperature = currentTemperature + temperatureChange;
    const double scaleFactor = std::sqrt(newTemperature / currentTemperature);
    for (auto &particle: particleContainer) {
        if (particle.getFixed()) continue;
        std::array<double, 3> newV = operator*(scaleFactor, particle.getV());
        particle.setV(newV);
    }


}
double DirectThermostat::getContainerKineticEnergy(ParticleContainers::ParticleContainer& particleContainer) {
    double total = 0;
    for (auto& particle : particleContainer) {
        if (particle.getFixed()) continue;
        std::array<double, 3> v = particle.getV();
        total += particle.getM() * (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) * 0.5;
    }
    return total;
}