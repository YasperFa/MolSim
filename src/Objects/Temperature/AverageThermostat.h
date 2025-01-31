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
    void applyThermostat(ParticleContainers::ParticleContainer& particleContainer) override;
    double getContainerKineticEnergy(ParticleContainers::ParticleContainer& particleContainer) override;

};