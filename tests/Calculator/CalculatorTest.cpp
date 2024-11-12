//
// Created by Yasmine Farah on 07/11/2024.
//
#include "gtest/gtest.h"
#include "../../src/Calculator/Calculator.h"
#include "../../src/Objects/ParticleContainer.h"
#include "../../src/Objects/Particle.h"

/*Checks that calculateX() correctly updates the position of a particle*/
TEST(CalculatorTest, correctXcalculations) {
    SPDLOG_DEBUG("running test");
    ParticleContainer test;
    SPDLOG_DEBUG("created container");
    Particle p({0.0, 0.0, 0.0},{0.1, 0.1, 0.1},1.0,0);
    SPDLOG_DEBUG("created particle");
    p.setF({0.2, 0.2, 0.2});
    test.addParticle(p);
    Calculators::Calculator calc;
    calc.calculateX(test,0.5); //value different from 1 to check that (delta_t)^2 is calculated correctly

    for (auto &t : test) {
        SPDLOG_DEBUG("Looking at particle");
        EXPECT_NEAR(0.075, t.getX()[0], 0.00001);
        EXPECT_NEAR(0.075, t.getX()[1], 0.00001);
        EXPECT_NEAR(0.075, t.getX()[2], 0.00001);
    }

}

/*Checks that calculateF() correctly updates the force between particles*/
TEST(CalculatorTest, correctFIJcalculations) {
    ParticleContainer test;
    std::array<double,3> old = {1.0, 2.0, 3.0};

    Particle i({0.0, 0.0, 0.0},{0.0, 0.0, 0.0},1.0,0);
    i.setF(old);
    test.addParticle(i);

    Particle j({2.0, 2.0, 2.0},{0.0, 0.0, 0.0},2.0,0);
    j.setF(old);
    test.addParticle(j);

    Particle k({4.0, 4.0, 4.0},{0.0, 0.0, 0.0},3.0,0);
    k.setF(old);
    test.addParticle(k);

    test.initializePairsVector();


    Calculators::Calculator calc;
    calc.calculateF(test); 

    for (Particle& p: test) {
        ASSERT_EQ(old, test.getParticle(p.getID()).getOldF());
    }  

    for (int in = 0; in < 3; in++) {
        EXPECT_NEAR(0.13230943, (test.getParticle(i).getF())[in], 0.00001);
        EXPECT_NEAR(0.19245008, (test.getParticle(j).getF())[in], 0.00001);
        EXPECT_NEAR(-0.3247695, (test.getParticle(k).getF())[in], 0.00001);
    }
}

/**Checks that calculateV() correctly updates the velocity of a particle*/
TEST(CalculatorTest, correctVcalculations) {
    ParticleContainer test;
    Particle p({0.0, 0.0, 0.0},{1.0, 1.0, 1.0}, 4.0 ,0);
    p.setOldF({2.0, 2.0, 2.0});
    p.setF({3.0, 3.0, 3.0});
    test.addParticle(p);
    Calculators::Calculator calc;
    calc.calculateV(test,0.5); //value different from 1 to check that (delta_t)^2 is calculated correctly

    for (auto &t : test) {
        EXPECT_NEAR(1.3125, t.getV()[0], 0.00001);
        EXPECT_NEAR(1.3125, t.getV()[1], 0.00001);
        EXPECT_NEAR(1.3125, t.getV()[2], 0.00001);
    }
}

    TEST(CalculatorTest, correctXFVcalculations) {
    ParticleContainer test;

    std::array<double, 3> a = {4.0, 4.0, 4.0};
    std::array<double, 3> b = {8.0, 8.0, 8.0};
    std::array<double, 3> c = {12.0, 12.0, 12.0};
    
    Particle i({1.0, 1.0, 1.0},{2.0, 2.0, 2.0},3.0,0);
    i.setF(a); //newF: 0.3518228203
    test.addParticle(i);

    Particle j({5.0, 5.0, 5.0},{6.0, 6.0, 6.0},7.0,0);
    j.setF(b); //newF: 0.6735753141
    test.addParticle(j);

    Particle k({9.0, 9.0, 9.0},{10.0, 10.0, 10.0}, 11.0,0);
    k.setF(c); //newF: -1.025398134
    test.addParticle(k);

    test.initializePairsVector();


    Calculators::Calculator calc;
    calc.calculateXFV(test, 0.5); 

    for (int in = 0; in < 3; in++) {
        ASSERT_EQ(a, test.getParticle(i.getID()).getOldF());
        ASSERT_EQ(b, test.getParticle(j.getID()).getOldF());
        ASSERT_EQ(c, test.getParticle(k.getID()).getOldF());

        EXPECT_NEAR(2.16666666, (test.getParticle(i).getX())[in], 0.00001);
        EXPECT_NEAR(8.14285714, (test.getParticle(j).getX())[in], 0.00001);
        EXPECT_NEAR(14.1363636, (test.getParticle(k).getX())[in], 0.00001);

        EXPECT_NEAR(0.15748559, (test.getParticle(i).getF())[in], 0.00001);
        EXPECT_NEAR(0.29936292, (test.getParticle(j).getF())[in], 0.00001);
        EXPECT_NEAR(-0.45684851, (test.getParticle(k).getF())[in], 0.00001);

        EXPECT_NEAR(2.346457132, (test.getParticle(i).getV())[in], 0.00001);
        EXPECT_NEAR(6.296405819, (test.getParticle(j).getV())[in], 0.00001);
        EXPECT_NEAR(10.26234435, (test.getParticle(k).getV())[in], 0.00001);
    }
    

}