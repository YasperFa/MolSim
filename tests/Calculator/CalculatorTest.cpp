//
// Created by Yasmine Farah on 07/11/2024.
//
#include "gtest/gtest.h"
#include "../../src/Calculator/Calculator.h"
#include "../../src/Calculator/GravityCalculator.h"
#include "../../src/Calculator/LennardJonesCalculator.h"
#include "../../src/Calculator/HarmonicForceCalculator.h"
#include "../../src/Objects/Containers/DirectSum/DirectSumContainer.h"
#include "../../src/Objects/Containers/LinkedCell/LinkedCellContainer.h"
#include "../../src/Objects/Particle.h"
#include "../../src/utils/ArrayUtils.h"


/*Checks that calculateX() correctly updates the position of a particle*/
TEST(CalculatorTest, correctXcalculations) {
    SPDLOG_DEBUG("running testContainer");
    ParticleContainers::DirectSumContainer testContainer;
    SPDLOG_DEBUG("created container");
    Particle p({0.0, 0.0, 0.0},{0.1, 0.1, 0.1},1.0,0);
    SPDLOG_DEBUG("created particle");
    p.setF({0.2, 0.2, 0.2});
    testContainer.addParticle(p);
    Calculators::GravityCalculator calc;
    calc.calculateX(testContainer,0.5); //value different from 1 to check that (delta_t)^2 is calculated correctly

    for (auto &t : testContainer) {
        SPDLOG_DEBUG("Looking at particle");
        EXPECT_NEAR(0.075, t.getX()[0], 0.00001);
        EXPECT_NEAR(0.075, t.getX()[1], 0.00001);
        EXPECT_NEAR(0.075, t.getX()[2], 0.00001);
    }

}

/*Checks that calculateF() correctly updates the force between particles*/
TEST(CalculatorTest, correctFIJcalculations) {
    ParticleContainers::DirectSumContainer testContainer;
    std::array<double,3> old = {1.0, 2.0, 3.0};

    Particle i({0.0, 0.0, 0.0},{0.0, 0.0, 0.0},1.0,0);
    i.setF(old);
    testContainer.addParticle(i);

    Particle j({2.0, 2.0, 2.0},{0.0, 0.0, 0.0},2.0,0);
    j.setF(old);
    testContainer.addParticle(j);

    Particle k({4.0, 4.0, 4.0},{0.0, 0.0, 0.0},3.0,0);
    k.setF(old);
    testContainer.addParticle(k);


    Calculators::GravityCalculator calc;
    calc.calculateF(testContainer); 

    for (Particle& p: testContainer) {
     EXPECT_EQ(old, testContainer.getParticle(p.getID()).getOldF());
    }  

    for (int in = 0; in < 3; in++) {
        EXPECT_NEAR(0.13230943, (testContainer.getParticle(i).getF())[in], 0.00001);
        EXPECT_NEAR(0.19245008, (testContainer.getParticle(j).getF())[in], 0.00001);
        EXPECT_NEAR(-0.3247695, (testContainer.getParticle(k).getF())[in], 0.00001);
    }
}

/*Checks that calculateF() correctly adds gravitational force*/
TEST(CalculatorTest, calculateFGravity) {
    ParticleContainers::DirectSumContainer testContainer;
    std::array<double,3> old = {1.0, 2.0, 3.0};

    Particle i({0.0, 0.0, 0.0},{0.0, 0.0, 0.0},1.0,0);
    i.setF(old);
    testContainer.addParticle(i);

    Particle j({2.0, 2.0, 2.0},{0.0, 0.0, 0.0},2.0,0);
    j.setF(old);
    testContainer.addParticle(j);

    Particle k({4.0, 4.0, 4.0},{0.0, 0.0, 0.0},3.0,0);
    k.setF(old);
    testContainer.addParticle(k);


    Calculators::GravityCalculator calc;
    calc.calculateF(testContainer, 2); 

    for (Particle& p: testContainer) {
     EXPECT_EQ(old, testContainer.getParticle(p.getID()).getOldF());
    }  


        EXPECT_NEAR(0.13230943, (testContainer.getParticle(i).getF())[0], 0.0001); //no added gravity
        EXPECT_NEAR(0.19245008, (testContainer.getParticle(j).getF())[0], 0.0001);
        EXPECT_NEAR(-0.3247695, (testContainer.getParticle(k).getF())[0], 0.0001);

        EXPECT_NEAR(2.13230943, (testContainer.getParticle(i).getF())[1], 0.0001); //added gravity
        EXPECT_NEAR(4.19245008, (testContainer.getParticle(j).getF())[1], 0.0001);
        EXPECT_NEAR(5.675205, (testContainer.getParticle(k).getF()) [1], 0.0001);

        EXPECT_NEAR(0.13230943, (testContainer.getParticle(i).getF())[2], 0.0001); //no added gravity
        EXPECT_NEAR(0.19245008, (testContainer.getParticle(j).getF())[2], 0.0001);
        EXPECT_NEAR(-0.3247695, (testContainer.getParticle(k).getF())[2], 0.0001);
}


/**Checks that calculateV() correctly updates the velocity of a particle*/
TEST(CalculatorTest, correctVcalculations) {
    ParticleContainers::DirectSumContainer testContainer;
    Particle p({0.0, 0.0, 0.0},{1.0, 1.0, 1.0}, 4.0 ,0);
    p.setOldF({2.0, 2.0, 2.0});
    p.setF({3.0, 3.0, 3.0});
    testContainer.addParticle(p);
    Calculators::GravityCalculator calc;
    calc.calculateV(testContainer,0.5); //value different from 1 to check that (delta_t)^2 is calculated correctly

    for (auto &t : testContainer) {
        EXPECT_NEAR(1.3125, t.getV()[0], 0.00001);
        EXPECT_NEAR(1.3125, t.getV()[1], 0.00001);
        EXPECT_NEAR(1.3125, t.getV()[2], 0.00001);
    }
}
// checks if x f v are calculated correctly using newton force calculation
    TEST(CalculatorTest, correctXFVcalculations) {
    ParticleContainers::DirectSumContainer testContainer;

    std::array<double, 3> a = {4.0, 4.0, 4.0};
    std::array<double, 3> b = {8.0, 8.0, 8.0};
    std::array<double, 3> c = {12.0, 12.0, 12.0};
    
    Particle i({1.0, 1.0, 1.0},{2.0, 2.0, 2.0},3.0,0);
    i.setF(a); //newF: 0.3518228203
    testContainer.addParticle(i);

    Particle j({5.0, 5.0, 5.0},{6.0, 6.0, 6.0},7.0,0);
    j.setF(b); //newF: 0.6735753141
    testContainer.addParticle(j);

    Particle k({9.0, 9.0, 9.0},{10.0, 10.0, 10.0}, 11.0,0);
    k.setF(c); //newF: -1.025398134
    testContainer.addParticle(k);


    Calculators::GravityCalculator calc;
    calc.calculateXFV(testContainer, 0.5); 

    for (int in = 0; in < 3; in++) {
        EXPECT_EQ(a, testContainer.getParticle(i.getID()).getOldF());
        EXPECT_EQ(b, testContainer.getParticle(j.getID()).getOldF());
        EXPECT_EQ(c, testContainer.getParticle(k.getID()).getOldF());

        EXPECT_NEAR(2.16666666, (testContainer.getParticle(i).getX())[in], 0.00001);
        EXPECT_NEAR(8.14285714, (testContainer.getParticle(j).getX())[in], 0.00001);
        EXPECT_NEAR(14.1363636, (testContainer.getParticle(k).getX())[in], 0.00001);

        EXPECT_NEAR(0.15748559, (testContainer.getParticle(i).getF())[in], 0.00001);
        EXPECT_NEAR(0.29936292, (testContainer.getParticle(j).getF())[in], 0.00001);
        EXPECT_NEAR(-0.45684851, (testContainer.getParticle(k).getF())[in], 0.00001);

        EXPECT_NEAR(2.346457132, (testContainer.getParticle(i).getV())[in], 0.00001);
        EXPECT_NEAR(6.296405819, (testContainer.getParticle(j).getV())[in], 0.00001);
        EXPECT_NEAR(10.26234435, (testContainer.getParticle(k).getV())[in], 0.00001);
    }

    }

    /*Checks that calculateF() of LJC correctly updates the force between particles for DirectSumContainer*/
TEST(CalculatorTest, correctLJcalculationsDirectSum) {
    ParticleContainers::DirectSumContainer testContainer;

    Particle i({0.0, 0.0, 0.0},{0.0, 0.0, 0.0},1.0,0);
    testContainer.addParticle(i);

    Particle j({2.0, 2.0, 2.0},{0.0, 0.0, 0.0},2.0,0);
    testContainer.addParticle(j);

    Particle k({4.0, 4.0, 4.0},{0.0, 0.0, 0.0},3.0,0);
    testContainer.addParticle(k);

    Calculators::LennardJonesCalculator calc = Calculators::LennardJonesCalculator(false,false);
    calc.calculateF(testContainer, 0);

    for (int in = 0; in < 3; in++) {
        EXPECT_NEAR(0.01165109898, (testContainer.getParticle(i).getF())[in], 0.00001);
        EXPECT_NEAR(0, (testContainer.getParticle(j).getF())[in], 0.00001);
        EXPECT_NEAR(-0.01165109898, (testContainer.getParticle(k).getF())[in], 0.00001);
    }
}

 /*Checks that calculateF() of LJC correctly updates the force between particles for LinkedCellContainer*/
TEST(CalculatorTest, correctLJcalculationsLinkedCell) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{100, 100, 10}, 5, false);

    Particle i({0.0, 0.0, 0.0},{0.0, 0.0, 0.0},1.0,0);
    testContainer.addParticle(i);

    Particle j({2.0, 2.0, 2.0},{0.0, 0.0, 0.0},2.0,0);
    testContainer.addParticle(j);

    Particle k({4.0, 4.0, 4.0},{0.0, 0.0, 0.0},3.0,0);
    testContainer.addParticle(k);

    Calculators::LennardJonesCalculator calc = Calculators::LennardJonesCalculator(false,false);
    calc.calculateF(testContainer, 0);

    for (int in = 0; in < 3; in++) {
        EXPECT_NEAR(0.01165109898, (testContainer.getParticles().at(0).getF())[in], 0.00001);
        EXPECT_NEAR(0, testContainer.getParticles().at(1).getF()[in], 0.00001);
        EXPECT_NEAR(-0.01165109898, testContainer.getParticles().at(2).getF()[in], 0.00001);
    }
    // test version2
    ParticleContainers::LinkedCellContainer testContainer2(std::array<double,3>{100, 100, 10}, 5, true);


    testContainer.addParticle(i);

    testContainer.addParticle(j);

    testContainer.addParticle(k);

    calc.calculateF(testContainer2, 0);
    for (int in = 0; in < 3; in++) {
        EXPECT_NEAR(0.01165109898, (testContainer.getParticles().at(0).getF())[in], 0.00001);
        EXPECT_NEAR(0, testContainer.getParticles().at(1).getF()[in], 0.00001);
        EXPECT_NEAR(-0.01165109898, testContainer.getParticles().at(2).getF()[in], 0.00001);
    }
}

/*Checks that calculateF()correctly updates the force between particles of different types*/
TEST(CalculatorTest, correctFijMixing) {
    ParticleContainers::DirectSumContainer testContainer;

    Particle i({0.0, 0.0, 0.0},{0.0, 0.0, 0.0},1.0,0, 5.0, 2.0);
    testContainer.addParticle(i);

    Particle j({1.0, 1.0, 1.0},{0.0, 0.0, 0.0},2.0,0, 5.5, 2.2);
    testContainer.addParticle(j);

    Particle k({2.0, 2.0, 2.0},{0.0, 0.0, 0.0},3.0,0, 5.0, 2.0);
    testContainer.addParticle(k);

    Calculators::LennardJonesCalculator calc = Calculators::LennardJonesCalculator(false,false);
    calc.calculateF(testContainer, 0);

    for (int in = 0; in < 3; in++) {
        EXPECT_NEAR(-712.6750486, (testContainer.getParticle(i).getF())[in], 0.00001);
        EXPECT_NEAR(0, (testContainer.getParticle(j).getF())[in], 0.00001);
        EXPECT_NEAR(712.6750486, (testContainer.getParticle(k).getF())[in], 0.00001);
    }
}

/** Test that `calculateFIJ` correctly computes harmonic forces. */
TEST(HarmonicForceCalculatorTest, CorrectForceCalculation) {
    // Initialize calculator with stiffness constant and average bond length
    double stiffnessConstant = 10.0;
    double avgBondLength = 1.0;
    Calculators::HarmonicForceCalculator calc(stiffnessConstant, avgBondLength);

    // Define particle positions
    std::array<double, 3> position1 = {0.0, 0.0, 0.0};
    std::array<double, 3> position2 = {1.5, 0.0, 0.0};

    // Subtract positions to calculate the displacement
    std::array<double, 3> sub = position2 - position1;

    // Create particles
    Particle particle1(position1, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle particle2(position2, {0.0, 0.0, 0.0}, 1.0, 1);

    // Calculate force
    std::array<double, 3> force = calc.calculateFIJ(sub, particle1, particle2, 1.0);

    // Verify the force
    EXPECT_NEAR(5.0, force[0], 1e-5); // Stiffness constant x displacement (1.5 - 1.0)
    EXPECT_NEAR(0.0, force[1], 1e-5);
    EXPECT_NEAR(0.0, force[2], 1e-5);
}

/** Test force calculation with a bond length multiplier. */
TEST(HarmonicForceCalculatorTest, ForceWithBondLengthMultiplier) {
    // Initialize calculator
    double stiffnessConstant = 15.0;
    double avgBondLength = 2.0;
    double bondLengthMultiplier = 0.5;
    Calculators::HarmonicForceCalculator calc(stiffnessConstant, avgBondLength);

    // Define particle positions
    std::array<double, 3> position1 = {0.0, 0.0, 0.0};
    std::array<double, 3> position2 = {3.0, 0.0, 0.0};

    // Displacement
    std::array<double, 3> sub = position2 - position1;

    // Particles
    Particle particle1(position1, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle particle2(position2, {0.0, 0.0, 0.0}, 1.0, 1);

    // Calculate force
    std::array<double, 3> force = calc.calculateFIJ(sub, particle1, particle2, bondLengthMultiplier);

    // Verify force
    EXPECT_NEAR(30.0, force[0], 1e-5); // Stiffness constant x (distance - bondLength)
    EXPECT_NEAR(0.0, force[1], 1e-5);
    EXPECT_NEAR(0.0, force[2], 1e-5);
}

/** Test `toString` method. */
TEST(HarmonicForceCalculatorTest, ToString) {
    Calculators::HarmonicForceCalculator calc(10.0, 1.0);
    EXPECT_EQ("Harmonic", calc.toString());
}

/** Test that no force is applied when particles are at equilibrium distance. */
TEST(HarmonicForceCalculatorTest, NoForceAtEquilibrium) {
    double stiffnessConstant = 20.0;
    double avgBondLength = 1.0;
    Calculators::HarmonicForceCalculator calc(stiffnessConstant, avgBondLength);

    std::array<double, 3> position1 = {0.0, 0.0, 0.0};
    std::array<double, 3> position2 = {1.0, 0.0, 0.0}; // At equilibrium distance

    std::array<double, 3> sub = position2 - position1;

    Particle particle1(position1, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle particle2(position2, {0.0, 0.0, 0.0}, 1.0, 1);

    std::array<double, 3> force = calc.calculateFIJ(sub, particle1, particle2, 1.0);

    EXPECT_NEAR(0.0, force[0], 1e-5);
    EXPECT_NEAR(0.0, force[1], 1e-5);
    EXPECT_NEAR(0.0, force[2], 1e-5);
}

/** Test that the force calculation works for particles displaced in multiple dimensions. */
TEST(HarmonicForceCalculatorTest, ForceInMultipleDimensions) {
    double stiffnessConstant = 10.0;
    double avgBondLength = 1.0;
    Calculators::HarmonicForceCalculator calc(stiffnessConstant, avgBondLength);

    std::array<double, 3> position1 = {0.0, 0.0, 0.0};
    std::array<double, 3> position2 = {1.2, 1.5, 0.0};

    std::array<double, 3> sub = position2 - position1;

    Particle particle1(position1, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle particle2(position2, {0.0, 0.0, 0.0}, 1.0, 1);

    std::array<double, 3> force = calc.calculateFIJ(sub, particle1, particle2, 1.0);

    double expectedForceMagnitude = stiffnessConstant * (std::sqrt(1.2 * 1.2 + 1.5 * 1.5) - avgBondLength) /
                                    std::sqrt(1.2 * 1.2 + 1.5 * 1.5);

    EXPECT_NEAR(expectedForceMagnitude * 1.2, force[0], 1e-5);
    EXPECT_NEAR(expectedForceMagnitude * 1.5, force[1], 1e-5);
    EXPECT_NEAR(0.0, force[2], 1e-5);
}


/** Test force calculation with a large bond length multiplier. */
TEST(HarmonicForceCalculatorTest, LargeBondLengthMultiplier) {
    double stiffnessConstant = 5.0;
    double avgBondLength = 1.0;
    double bondLengthMultiplier = 3.0;
    Calculators::HarmonicForceCalculator calc(stiffnessConstant, avgBondLength);

    std::array<double, 3> position1 = {0.0, 0.0, 0.0};
    std::array<double, 3> position2 = {4.0, 0.0, 0.0};

    std::array<double, 3> sub = position2 - position1;

    Particle particle1(position1, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle particle2(position2, {0.0, 0.0, 0.0}, 1.0, 1);

    std::array<double, 3> force = calc.calculateFIJ(sub, particle1, particle2, bondLengthMultiplier);

    EXPECT_NEAR(5.0, force[0], 1e-5); // (4.0 - 3.0) * stiffnessConstant
    EXPECT_NEAR(0.0, force[1], 1e-5);
    EXPECT_NEAR(0.0, force[2], 1e-5);
}

/** Test force calculation with negative stiffness constant. */
TEST(HarmonicForceCalculatorTest, NegativeStiffnessConstant) {
    double stiffnessConstant = -10.0; // Negative stiffness
    double avgBondLength = 1.0;
    Calculators::HarmonicForceCalculator calc(stiffnessConstant, avgBondLength);

    std::array<double, 3> position1 = {0.0, 0.0, 0.0};
    std::array<double, 3> position2 = {1.5, 0.0, 0.0};

    std::array<double, 3> sub = position2 - position1;

    Particle particle1(position1, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle particle2(position2, {0.0, 0.0, 0.0}, 1.0, 1);

    std::array<double, 3> force = calc.calculateFIJ(sub, particle1, particle2, 1.0);

    EXPECT_NEAR(-5.0, force[0], 1e-5); // Stiffness constant x displacement (1.5 - 1.0)
    EXPECT_NEAR(0.0, force[1], 1e-5);
    EXPECT_NEAR(0.0, force[2], 1e-5);
}

/** Test that particles at a large distance produce the expected force. */
TEST(HarmonicForceCalculatorTest, LargeDistance) {
    double stiffnessConstant = 8.0;
    double avgBondLength = 1.0;
    Calculators::HarmonicForceCalculator calc(stiffnessConstant, avgBondLength);

    std::array<double, 3> position1 = {0.0, 0.0, 0.0};
    std::array<double, 3> position2 = {10.0, 0.0, 0.0}; // Far apart

    std::array<double, 3> sub = position2 - position1;

    Particle particle1(position1, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle particle2(position2, {0.0, 0.0, 0.0}, 1.0, 1);

    std::array<double, 3> force = calc.calculateFIJ(sub, particle1, particle2, 1.0);

    EXPECT_NEAR(72.0, force[0], 1e-5); // Stiffness constant x (distance - bondLength)
    EXPECT_NEAR(0.0, force[1], 1e-5);
    EXPECT_NEAR(0.0, force[2], 1e-5);
}

/** Test applying harmonic forces with direct neighbors. */
TEST(HarmonicForceApplicationTest, DirectNeighborsForceApplication) {
    ParticleContainers::DirectSumContainer container;
    Calculators::LennardJonesCalculator calcLJ = Calculators::LennardJonesCalculator(true, true);

    // Define particles
    Particle particle1({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle particle2({2.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 1);

    // Set neighbors
    particle1.addDirectNeighbour(particle2.getID());
    particle2.addDirectNeighbour(particle1.getID());

    // Add particles to container
    container.addParticle(particle1);
    container.addParticle(particle2);

    // Create harmonic force calculator
    double stiffnessConstant = 10.0;
    double avgBondLength = 1.0;
    Calculators::HarmonicForceCalculator calc(stiffnessConstant, avgBondLength);

    // Apply harmonic forces
    calcLJ.applyHarmonicForces(container, calc);

    // Verify forces
    for (auto &particle : container) {
        if (particle.getID() == 1) {
            EXPECT_NEAR(10.0, particle.getF()[0], 1e-5);
            EXPECT_NEAR(0.0, particle.getF()[1], 1e-5);
            EXPECT_NEAR(0.0, particle.getF()[2], 1e-5);
        } else if (particle.getID() == 2) {
            EXPECT_NEAR(-10.0, particle.getF()[0], 1e-5);
            EXPECT_NEAR(0.0, particle.getF()[1], 1e-5);
            EXPECT_NEAR(0.0, particle.getF()[2], 1e-5);
        }
    }
}

/** Test applying harmonic forces with diagonal neighbors. */
TEST(HarmonicForceApplicationTest, DiagonalNeighborsForceApplication) {
    ParticleContainers::DirectSumContainer container;
    Calculators::LennardJonesCalculator calcLJ = Calculators::LennardJonesCalculator(true, true);


    // Define particles
    Particle particle1({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle particle2({1.0, 1.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 1);

    // Set diagonal neighbors
    particle1.addDiagonalNeighbour(particle2.getID());
    particle2.addDiagonalNeighbour(particle1.getID());

    // Add particles to container
    container.addParticle(particle1);
    container.addParticle(particle2);

    // Create harmonic force calculator
    double stiffnessConstant = 5.0;
    double avgBondLength = 1.5;
    Calculators::HarmonicForceCalculator calc(stiffnessConstant, avgBondLength);

    // Apply harmonic forces
    calcLJ.applyHarmonicForces(container, calc);

    // Verify forces
    for (auto &particle : container) {
        if (particle.getID() == 1) {
            EXPECT_NEAR(-2.5, particle.getF()[0], 1e-5); // Scaled by sqrt(2)
            EXPECT_NEAR(-2.5, particle.getF()[1], 1e-5);
            EXPECT_NEAR(0.0, particle.getF()[2], 1e-5);
        } else if (particle.getID() == 2) {
            EXPECT_NEAR(2.5, particle.getF()[0], 1e-5);
            EXPECT_NEAR(2.5, particle.getF()[1], 1e-5);
            EXPECT_NEAR(0.0, particle.getF()[2], 1e-5);
        }
    }
}

/** Test upward force applied to marked particles */
TEST(CalculateFTest, UpwardForceOnMarkedParticles) {
    ParticleContainers::DirectSumContainer testContainer;
    Calculators::LennardJonesCalculator calcLJ = Calculators::LennardJonesCalculator(true, true);

    // Create particles
    Particle p1({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 0);
    p1.markForUpwardForce();
    Particle p2({1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, 2.0, 0); // Not marked
    p1.addDiagonalNeighbour(p2.getID());
    p2.addDiagonalNeighbour(p1.getID());

    testContainer.addParticle(p1);
    testContainer.addParticle(p2);

    // Apply upward force
    double upwardForce = 5.0;
    calcLJ.calculateF(testContainer, 0.0, false, 0.0, 0.0, upwardForce, 1, 1, 1);

    // Verify forces
    EXPECT_NEAR(5., testContainer.getParticles()[0].getF()[1], 1e-5);
    EXPECT_NEAR(0., testContainer.getParticles()[1].getF()[1], 1e-5);
}

/** Test when no upward force is applied */
TEST(CalculateFTest, NoUpwardForce) {
    ParticleContainers::DirectSumContainer testContainer;
    Calculators::GravityCalculator calc;


    // Create particles
    Particle p1({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 0);
    Particle p2({1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, 2.0, 0);

    testContainer.addParticle(p1);
    testContainer.addParticle(p2);

    // Apply no upward force
    calc.calculateF(testContainer, 0.0, false, 0.0, 0.0, 0.0, 1, 1, 1);

    // Verify forces
    EXPECT_NEAR(0.3849, testContainer.getParticles()[0].getF()[1], 1e-5);
    EXPECT_NEAR(-0.3849, testContainer.getParticles()[1].getF()[1], 1e-5);
}

/** Test combined gravity and upward force */
TEST(CalculateFTest, CombinedGravityAndUpwardForce) {
    ParticleContainers::DirectSumContainer testContainer;
    Calculators::GravityCalculator calc;

    // Create particles
    Particle p1({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 0);
    p1.markForUpwardForce();
    Particle p2({1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, 2.0, 0);

    testContainer.addParticle(p1);
    testContainer.addParticle(p2);

    // Apply gravity and upward force
    double gravity = -9.8;
    double upwardForce = 5.0;
    calc.calculateF(testContainer, gravity, false, 0.0, 0.0, upwardForce, 1, 1, 1);

    // Verify forces
    EXPECT_NEAR(-4.41509, testContainer.getParticles()[0].getF()[1], 1e-5); // Upward force + gravity
    EXPECT_NEAR(-19.9849, testContainer.getParticles()[1].getF()[1], 1e-5); // Gravity only
}

/** Test upward force on a different axis */
TEST(CalculateFTest, UpwardForceOnDifferentAxis) {
    ParticleContainers::DirectSumContainer testContainer;
    Calculators::GravityCalculator calc;


    // Create particles
    Particle p1({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 0);
    p1.markForUpwardForce();
    Particle p2({1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, 2.0, 0);

    testContainer.addParticle(p1);
    testContainer.addParticle(p2);

    // Apply upward force on the x-axis
    double upwardForce = 10.0;
    calc.calculateF(testContainer, 0.0, false, 0.0, 0.0, upwardForce, 1, 0, 0);

    // Verify forces
    EXPECT_NEAR(10.3849, testContainer.getParticles()[0].getF()[0], 1e-5);
    EXPECT_NEAR(-0.3849, testContainer.getParticles()[1].getF()[0], 1e-5);
}
