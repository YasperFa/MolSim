//
// Created by yasmine Farah on 06/12/2024.
//

#include <gtest/gtest.h>
#include "../../../src/Objects/Temperature/Thermostat.h"
#include "../../../src/Objects/Temperature/DirectThermostat.h"
#include "../../../src/Objects/Containers/DirectSum/DirectSumContainer.h"

// test if kinetic energy is calculated correctly
TEST(ThermostatTest, getKineticEnergyTest) {
    ParticleContainers::DirectSumContainer testContainer;
    testContainer.addParticle(Particle({2, 3, 0}, {5, 1.1, 0}, 2, 0));
    testContainer.addParticle(Particle({2, 1, 0}, {0, -4.1, 0}, 1, 0));
    testContainer.addParticle(Particle({1, 2, 0}, {-1.3, 0, 0}, 1, 0));
    testContainer.addParticle(Particle({3, 2, 0}, {1.0, 0, 0}, 1, 0));
    DirectThermostat t(10,20, false, 1);
    EXPECT_NEAR(t.getContainerKineticEnergy(testContainer), 35.96, 0.00001);
}
// test correct scaling for heating -- directThermostat
TEST(DirectThermostatTest, applyDirectThermostatHeating) {
    ParticleContainers::DirectSumContainer testContainer;
    testContainer.addParticle(Particle({2, 3, 0}, {5, 1.1, 0}, 2, 0));
    testContainer.addParticle(Particle({2, 1, 0}, {0, -4.1, 0}, 1, 0));
    testContainer.addParticle(Particle({1, 2, 0}, {-1.3, 0, 0}, 1, 0));
    testContainer.addParticle(Particle({3, 2, 0}, {1.0, 0, 0}, 1, 0));
    DirectThermostat t(10,20, false, 1);
    double currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 8.99 , 0.00001);
    t.applyThermostat(testContainer);
    currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 10.0, 0.00001);
    DirectThermostat t1(20,20, false, 1);
    t1.applyThermostat(testContainer);
    currentTemperature = t1.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 20.0, 0.00001);
    DirectThermostat t2(60,20, false, 1);
    double prevTemperature = currentTemperature;
    t2.applyThermostat(testContainer);
    currentTemperature = t2.getCurrentTemperature(testContainer, false);
    // maximum deltaT is 20
    EXPECT_NEAR(currentTemperature, prevTemperature + 20.0, 0.00001);
}
// test correct scaling for cooling -- directThermostat
TEST(DirectThermostatTest, applyDirectThermostatCooling) {
    ParticleContainers::DirectSumContainer testContainer;
    testContainer.addParticle(Particle({2, 3, 0}, {5, 1.1, 0}, 2, 0));
    testContainer.addParticle(Particle({2, 1, 0}, {0, -4.1, 0}, 1, 0));
    testContainer.addParticle(Particle({1, 2, 0}, {-1.3, 0, 0}, 1, 0));
    testContainer.addParticle(Particle({3, 2, 0}, {1.0, 0, 0}, 1, 0));
    DirectThermostat t(7,20, false, 1);
    double currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 8.99 , 0.00001);
    t.applyThermostat(testContainer);
    currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 7.0, 0.00001);
    DirectThermostat t1(1.0,20, false, 1);
    t1.applyThermostat(testContainer);
    currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 1.0, 0.00001);
    // set temperature to 60
    DirectThermostat t1_setTemp(60,100, false, 1);
    t1_setTemp.applyThermostat(testContainer);
    DirectThermostat t2(39.99,20, false, 1);
    t2.applyThermostat(testContainer);
    currentTemperature = t2.getCurrentTemperature(testContainer, false);
    // maximum deltaT is 20
    EXPECT_NEAR(currentTemperature,40.0, 0.00001);
}

