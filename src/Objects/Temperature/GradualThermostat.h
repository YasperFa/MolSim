//
// Created by Yasmine Farah on 05/12/2024.
//

#pragma once
#include "Thermostat.h"


/**
 * @brief control the temperature of a particle container gradually.
 */
class GradualThermostat : public Thermostat {
   public:
    GradualThermostat(const double targetTemperature,const double maxDeltaT,const
        bool is3D, const int n) : Thermostat(targetTemperature, maxDeltaT, is3D, n){}
    ~GradualThermostat() override = default;

 /**
* @brief scales the velocity according to the desired method.
* @param  currentTemperature: current temperature of the container
* @param  averageVelocity: average velocity of particles in the container
*/
  double getScale(double currentTemperature, std::array<double, 3> averageVelocity) override;



};


