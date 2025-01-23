//
// Created by Yasmine Farah on 01/11/2024.
//
#include "gtest/gtest.h"
#include "../../src/Objects/Containers/DirectSum/DirectSumContainer.h"
#include "../src/Objects/Particle.h"
#include "Objects/Containers/LinkedCell/LinkedCellContainer.h"
#include "spdlog/spdlog.h"
#include "Calculator/LennardJonesCalculator.h"
#include "utils/ArrayUtils.h"

/** Checks if sizeParticles() and addParticle() of DirectSumContainer work correctly */
TEST(DirectSumContainerTest, StrctureAfterAddParticle) {
    ParticleContainers::DirectSumContainer testContainer;
    Particle p(0);
    
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    testContainer.addParticle(p);
    EXPECT_EQ(testContainer.sizeParticles(), 1);

    testContainer.removeParticle(p);
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    

}

/** Checks if sizeParticles() and addParticle() of LinkedCellContainer work correctly and that particles are inserted in the correct cells*/
TEST(LinkedCellContainerTest, StrctureAfterAddParticle) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{8,8,1}, 1.0, false);
    Particle i({-1, -1, 0.0},{0.0, 0.0, 0.0},1.0,0);
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    testContainer.addParticle(i);
    EXPECT_EQ(testContainer.sizeParticles(), 1);
    testContainer.removeParticle(i);
    EXPECT_EQ(testContainer.sizeParticles(), 0);

    ParticleIdInitializer::reset();

    for (int i = 1; i < 11; i++){

    Particle particle({-1, i - 1.5, 0.0},{0.0, 0.0, 0.0}, 1.0, 0);
    testContainer.addParticle(particle);
    EXPECT_EQ(testContainer.sizeParticles(), i);

    bool contains = false;
    std::vector<Particle*> particlesInCell = testContainer.getCells().at(i - 1).getParticlesInCell();
    for (auto p : particlesInCell){
        if (p->getID() == i){
            contains = true;
            break;
        }
    }

    EXPECT_TRUE(contains);
}
}

/** Checks that cells in the LinkedCellContainer are initialized correctly for 2D simulations*/
TEST(LinkedCellContainerTest, correctCellInitialization2D) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{180,90,1}, 3.0, false);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[0], 60);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[1], 30);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[2], 1);
    // 62*32*1 = 1984
    EXPECT_EQ(testContainer.getCells().size(), 1984);
    // 2*60 + 2*30 - 4
    EXPECT_EQ(testContainer.getBoundaryCells().size(), 176);
    // 60*30
    EXPECT_EQ(testContainer.getInnerCells().size(), 1800);
    EXPECT_EQ(testContainer.getHaloCells().size(), 184);
}

/** Checks that only neighboring cells are influenced during calculations */
TEST(LinkedCellContainerTest, correctCalculations) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{5, 5, 1}, 1.0, false);
    Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator(false);
    

    Particle p({0, 2, 0.0},{0.0, 0.0, 0.0},2.0,0);
    testContainer.addParticle(p);
    Particle q({0, 3, 0.0},{0.0, 0.0, 0.0},1.0,0); //influenced by two
    testContainer.addParticle(q);
    Particle r({0, 4, 0.0},{0.0, 0.0, 0.0},1.0,0);
    testContainer.addParticle(r);

    Particle s({4, 4, 0.0},{0, 0, 0},1.0,0); //isolated
    testContainer.addParticle(s);

    Particle t({4, 0, 0.0},{0.0, 0.0, 0.0},1.0,0); //pair that influences each other
    testContainer.addParticle(q);
    Particle u({3, 0, 0.0},{0.0, 0.0, 0.0},1.0,0);
    testContainer.addParticle(r);

    calculator.calculateXFV(testContainer, 1, 0);

    EXPECT_EQ(q.getF(), -1 * (r.getF() + p.getF()));

    std::array<double, 3> expected = {0, 0, 0};
    EXPECT_EQ(s.getF(), expected);

    EXPECT_EQ(t.getF(), -1 * (u.getF()));
    
}

/* Tests that ids of new particles get initialized correctly */
TEST(ParticleIdInitializerTest, testIDsAreUnique) {
    ParticleIdInitializer::reset();
    Particle p(1);
    EXPECT_EQ(p.getID(), 1);

    Particle q(1);
    EXPECT_EQ(q.getID(), 2);

    Particle r = Particle(p); //creating a particle from copy should copy the id
    EXPECT_EQ(r.getID(), 1);

    Particle s(1); //creating particles from copy should not influence new ids
    EXPECT_EQ(s.getID(), 3);
}