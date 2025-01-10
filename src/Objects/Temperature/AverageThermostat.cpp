#include "AverageThermostat.h"
//
// Created by Sawsan Farah on 08/01/2025.
//
std::array<double, 3> AverageThermostat::getNewVel(double currentTemperature, std::array<double, 3> particleVelocity, std::array<double, 3> averageVelocity) {
    double temperatureChange;
    if (targetTemperature > currentTemperature) {
        temperatureChange = std::min(targetTemperature - currentTemperature, maxDeltaT);
    } else {
        temperatureChange = std::max(targetTemperature - currentTemperature, -maxDeltaT);
    }

    const double newTemperature = currentTemperature + temperatureChange;
    const double scaleFactor = std::sqrt(newTemperature / currentTemperature);
    std::array<double, 3> newV = operator+(averageVelocity , operator*(scaleFactor , (operator-(particleVelocity,averageVelocity))));
    return newV;
}