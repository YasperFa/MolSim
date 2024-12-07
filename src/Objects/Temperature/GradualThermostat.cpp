//
// Created by Yasmine Farah on 05/12/2024.
//

#include "GradualThermostat.h"
#include "utils/ArrayUtils.h"

 double GradualThermostat::getScale(double currentTemperature, std::array<double, 3> averageVelocity) {
    double temperatureChange;
    if (targetTemperature > currentTemperature) {
        temperatureChange = std::min(targetTemperature - currentTemperature, maxDeltaT);
        const double newTemperature = currentTemperature + temperatureChange;
        const double scaleFactor = std::sqrt(newTemperature / currentTemperature);
        const double gradualScale = std::sqrt(averageVelocity[0] * averageVelocity[0] + averageVelocity[1] * averageVelocity[1] + averageVelocity[2] * averageVelocity[2]);
        return 1 + (scaleFactor - 1)/gradualScale;
    }
     temperatureChange = std::max(targetTemperature - currentTemperature, -maxDeltaT);
    const double newTemperature = currentTemperature + temperatureChange;
    const double scaleFactor = std::sqrt(newTemperature / currentTemperature);
    const double gradualScale = std::sqrt(averageVelocity[0] * averageVelocity[0] + averageVelocity[1] * averageVelocity[1] + averageVelocity[2] * averageVelocity[2]);
    return scaleFactor + (1 - scaleFactor)/gradualScale;
}
