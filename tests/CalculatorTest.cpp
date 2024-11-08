//
// Created by Yasmine Farah on 07/11/2024.
//
#include "gtest/gtest.h"
#include "../src/Calculator/Calculator.h"
#include "../src/Objects/ParticleContainer.h"
#include "../src/Objects/Particle.h"

TEST(CalculatorTest, correctXcalculations) {
    ParticleContainer test;
    Particle p({0.0, 0.0, 0.0},{0.1, 0.1, 0.1},1.0,0);
    p.setF({0.1, 0.1, 0.1});
    test.addParticle(p);
    Calculators::Calculator calc;
    calc.calculateX(test,1.0);

    for (auto &t : test) {
        EXPECT_NEAR(0.15, t.getX()[0], 0.00001);
        EXPECT_NEAR(0.15, t.getX()[1], 0.00001);
        EXPECT_NEAR(0.15, t.getX()[2], 0.00001);
    }

}