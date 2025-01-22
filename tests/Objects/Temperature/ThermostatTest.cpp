//
// Created by yasmine Farah on 06/12/2024.
//

#include <gtest/gtest.h>
#include "../../../src/Objects/Temperature/Thermostat.h"
#include "../../../src/Objects/Temperature/DirectThermostat.h"
#include "../../../src/Objects/Containers/DirectSum/DirectSumContainer.h"
#include "Objects/Temperature/AverageThermostat.h"
#include "Objects/Temperature/ZXThermostat.h"

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
// test correct scaling for holding temperature -- directThermostat
TEST(DirectThermostatTest, applyDirectThermostatHold) {
    ParticleContainers::DirectSumContainer testContainer;
    testContainer.addParticle(Particle({2, 3, 0}, {5, 1.1, 0}, 2, 0));
    testContainer.addParticle(Particle({2, 1, 0}, {0, -4.1, 0}, 1, 0));
    testContainer.addParticle(Particle({1, 2, 0}, {-1.3, 0, 0}, 1, 0));
    testContainer.addParticle(Particle({3, 2, 0}, {1.0, 0, 0}, 1, 0));
    DirectThermostat t(8.99,20, false, 1);
    double currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 8.99 , 0.00001);
    t.applyThermostat(testContainer);
    currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 8.99, 0.00001);
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
// test correct scaling for heating -- AverageThermostat
TEST(AverageThermostatTest, applyAverageThermostatHeating) {
    ParticleContainers::DirectSumContainer testContainer;
    testContainer.addParticle(Particle({2, 3, 0}, {5, 1.1, 0}, 2, 0));
    testContainer.addParticle(Particle({2, 1, 0}, {0, -4.1, 0}, 1, 0));
    testContainer.addParticle(Particle({1, 2, 0}, {-1.3, 0, 0}, 1, 0));
    testContainer.addParticle(Particle({3, 2, 0}, {1.0, 0, 0}, 1, 0));
    AverageThermostat t(10,20, false, 1);
    double currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 8.99 , 0.00001);
    t.applyThermostat(testContainer);
    currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_LE(currentTemperature, 10.0);
    EXPECT_GT(currentTemperature, 8.99);
    double prevTemperature = t.getCurrentTemperature(testContainer, false);
    t.applyThermostat(testContainer);
    currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_LE(currentTemperature, 10.0);
    EXPECT_GT(currentTemperature, prevTemperature);

}
// test correct scaling for cooling -- AverageThermostat
TEST(AverageThermostatTest, applyAverageThermostatCooling) {
    ParticleContainers::DirectSumContainer testContainer;
    testContainer.addParticle(Particle({2, 3, 0}, {5, 1.1, 0}, 2, 0));
    testContainer.addParticle(Particle({2, 1, 0}, {0, -4.1, 0}, 1, 0));
    testContainer.addParticle(Particle({1, 2, 0}, {-1.3, 0, 0}, 1, 0));
    testContainer.addParticle(Particle({3, 2, 0}, {1.0, 0, 0}, 1, 0));
    AverageThermostat t(7,20, false, 1);
    double currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 8.99 , 0.00001);
    t.applyThermostat(testContainer);
    currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_GT(currentTemperature, 7.0);
    EXPECT_LE(currentTemperature, 8.99);
    double prevTemperature = t.getCurrentTemperature(testContainer, false);
    t.applyThermostat(testContainer);
    currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_GT(currentTemperature, 7.0);
    EXPECT_LE(currentTemperature, prevTemperature);

}
TEST(AverageThermostatTest, applyAverageThermostatHold) {
    ParticleContainers::DirectSumContainer testContainer;
    testContainer.addParticle(Particle({2, 3, 0}, {5, 1.1, 0}, 2, 0));
    testContainer.addParticle(Particle({2, 1, 0}, {0, -4.1, 0}, 1, 0));
    testContainer.addParticle(Particle({1, 2, 0}, {-1.3, 0, 0}, 1, 0));
    testContainer.addParticle(Particle({3, 2, 0}, {1.0, 0, 0}, 1, 0));
    AverageThermostat t(8.99,20, false, 1);
    double currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 8.99 , 0.00001);
    t.applyThermostat(testContainer);
    currentTemperature = t.getCurrentTemperature(testContainer, false);
    EXPECT_NEAR(currentTemperature, 8.99 , 0.00001);

}
//tests for correct calculation AverageThermostat
TEST(AverageThermostatTest, correctVelocityCalculationsAverageThermostat) {
    ParticleContainers::DirectSumContainer testContainer;
    testContainer.addParticle(Particle({0, 3, 0}, {0, 1.1, 1}, 2, 0));
    testContainer.addParticle(Particle({2, 1, 0}, {0, -4.1, 0}, 1, 0));
    testContainer.addParticle(Particle({1, 2, 0}, {-1.3, 0, 2}, 4, 0));
    testContainer.addParticle(Particle({3, 2, 0}, {1.0, 0, 3}, 1, 0));
    AverageThermostat t(10,20, true, 1);
    double currentTemperature = t.getCurrentTemperature(testContainer, true);
    EXPECT_NEAR(currentTemperature, 4.499166667 , 0.00001);
    std::array<double, 3> averageVelocity = t.getAverageVelocity(testContainer);
    EXPECT_NEAR(averageVelocity[0], -0.075 , 0.00001);
    EXPECT_NEAR(averageVelocity[1], -0.75 , 0.00001);
    EXPECT_NEAR(averageVelocity[2], 1.5 , 0.00001);
    double scale = 1.490850033;
    std::array<std::array<double,3>, 4> result_values = {};
    result_values[0] =  {(0+0.075)*scale - 0.075,(1.1+0.75)*scale - 0.75, (1-1.5)*scale + 1.5};
    result_values[1] =  {(0+0.075)*scale - 0.075,(-4.1+0.75)*scale - 0.75, (0-1.5)*scale + 1.5};
    result_values[2] =  {(-1.3+0.075)*scale - 0.075,(0+0.75)*scale - 0.75, (2-1.5)*scale + 1.5};
    result_values[3] =  {(1.0+0.075)*scale - 0.075,(0+0.75)*scale - 0.75, (3-1.5)*scale + 1.5};

    t.applyThermostat(testContainer);
    int i = 0;
    for (Particle p : testContainer) {
         EXPECT_NEAR(result_values[i][0],p.getV()[0],0.00001);
        EXPECT_NEAR(result_values[i][1],p.getV()[1],0.00001);
        EXPECT_NEAR(result_values[i][2],p.getV()[2],0.00001);
        i++;
    }

}
//tests for correct calculation ZX thermostat
TEST(ZXThermostatTest, correctVelocityCalculationsZxThermostat) {
    ParticleContainers::DirectSumContainer testContainer;
    testContainer.addParticle(Particle({0, 3, 0}, {0, 1.1, 1}, 2, 0));
    testContainer.addParticle(Particle({2, 1, 0}, {0, -4.1, 0}, 1, 0));
    testContainer.addParticle(Particle({1, 2, 0}, {-1.3, 0, 2}, 4, 0));
    testContainer.addParticle(Particle({3, 2, 0}, {1.0, 0, 3}, 1, 0));
    ZXThermostat t(10,20, true, 1);

    std::array<double, 3> averageVelocity = t.getAverageVelocity(testContainer);
    EXPECT_NEAR(averageVelocity[0], -0.075 , 0.00001);
    EXPECT_NEAR(averageVelocity[1], -0.75 , 0.00001);
    EXPECT_NEAR(averageVelocity[2], 1.5 , 0.00001);
    double keneticEnergy = 27.82;
     double currentTemperature = 2 * keneticEnergy / (3 * 4);
    double scale = std::sqrt(10/currentTemperature);
    std::array<std::array<double,3>, 4> result_values = {};
    result_values[0] =  {(0)*scale ,1.1, (1)*scale};
    result_values[1] =  {(0)*scale,-4.1, (0)*scale};
    result_values[2] =  {(-1.3)*scale ,0, (2)*scale};
    result_values[3] =  {(1.0)*scale,0, (3)*scale};

    t.applyThermostat(testContainer);
    int i = 0;
    for (Particle p : testContainer) {
        EXPECT_NEAR(result_values[i][0],p.getV()[0],0.00001);
        EXPECT_NEAR(result_values[i][1],p.getV()[1],0.00001);
        EXPECT_NEAR(result_values[i][2],p.getV()[2],0.00001);
        i++;
    }

}