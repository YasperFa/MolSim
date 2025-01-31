//
// Created by Yasmine Farah on 18/01/2025.
//
#pragma once
#include "Thermostat.h"

class ZXThermostat  : public Thermostat{
public:
    ZXThermostat(const double targetTemperature,const double maxDeltaT,const
    bool is3D, const int n) : Thermostat(targetTemperature, maxDeltaT, is3D, n){}
    ~ZXThermostat() override = default;
    void applyThermostat(ParticleContainers::ParticleContainer& particleContainer) override;
    double getContainerKineticEnergy(ParticleContainers::ParticleContainer& particleContainer) override;

};

