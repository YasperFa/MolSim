//
// Created by Yasmine Farah on 08/01/2025.
//

#pragma once
#include "Thermostat.h"
/**
 * @brief control the temperature of a particle container directly to reach target temperature.
 */
class AverageThermostat : public Thermostat{
public:
    AverageThermostat(const double targetTemperature,const double maxDeltaT,const
    bool is3D, const int n) : Thermostat(targetTemperature, maxDeltaT, is3D, n){}
    ~AverageThermostat() override = default;
        /**
      * @brief scales the velocity according to the desired method.
      * @param currentTemperature
      * @param  particleVelocity: velocity of the particle which we want to scale
      * @param averageVelocity
     */
    std::array<double, 3> getNewVel(double currentTemperature, std::array<double, 3> particleVelocity, std::array<double, 3> averageVelocity) override;

};