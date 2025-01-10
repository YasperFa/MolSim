//
// Created by Yasmine Farah on 05/12/2024.
//

#include "DirectThermostat.h"
#include "utils/ArrayUtils.h"


std::array<double, 3> DirectThermostat::getNewVel(double currentTemperature, std::array<double, 3> particleVelocity, std::array<double, 3> averageVelocity) {
    double temperatureChange;
    if (targetTemperature > currentTemperature) {
         temperatureChange = std::min(targetTemperature - currentTemperature, maxDeltaT);
    } else {
         temperatureChange = std::max(targetTemperature - currentTemperature, -maxDeltaT);
    }

    const double newTemperature = currentTemperature + temperatureChange;
    const double scaleFactor = std::sqrt(newTemperature / currentTemperature);
    std::array<double, 3> newV = operator*(scaleFactor, particleVelocity);
    return newV;
}