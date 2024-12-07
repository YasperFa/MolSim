//
// Created by Yasmine Farah on 05/12/2024.
//

#include "DirectThermostat.h"
#include "utils/ArrayUtils.h"


double DirectThermostat::getScale(double currentTemperature, std::array<double, 3> averageVelocity) {
    double temperatureChange;
    if (targetTemperature > currentTemperature) {
         temperatureChange = std::min(targetTemperature - currentTemperature, maxDeltaT);
    } else {
         temperatureChange = std::max(targetTemperature - currentTemperature, -maxDeltaT);
    }

    const double newTemperature = currentTemperature + temperatureChange;
    const double scaleFactor = std::sqrt(newTemperature / currentTemperature);
    return scaleFactor;
}