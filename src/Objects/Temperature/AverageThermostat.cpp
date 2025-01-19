#include "AverageThermostat.h"
//
// Created by Sawsan Farah on 08/01/2025.
//
void AverageThermostat::applyThermostat(ParticleContainers::ParticleContainer& particleContainer) {
    std::array<double, 3> averageVelocity = getAverageVelocity(particleContainer);
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
        std::array<double, 3> newV =  operator+(averageVelocity , operator*(scaleFactor , (operator-(particle.getV(),averageVelocity))));
        particle.setV(newV);
    }

}