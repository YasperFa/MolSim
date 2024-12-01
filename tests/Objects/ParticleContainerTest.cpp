//
// Created by Yasmine Farah on 01/11/2024.
//
#include "gtest/gtest.h"
#include "../../src/Objects/Containers/DirectSum/DirectSumContainer.h"
#include "../src/Objects/Particle.h"
#include "Objects/Containers/LinkedCell/LinkedCellContainer.h"

/* Checks if sizeParticles() and addParticle() works correctly */
TEST(DirectSumContainerTest, StrctureAfterAddParticle) {
    ParticleContainers::DirectSumContainer testContainer;
    Particle p(0);
    
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    testContainer.addParticle(p);
    EXPECT_EQ(testContainer.sizeParticles(), 1);

    testContainer.removeParticle(p);
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    

}
TEST(LinkedCellContainerTest, StrctureAfterAddParticle) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{180,90,1}, 3.0);
    Particle p(0);
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    testContainer.addParticle(p);
    EXPECT_EQ(testContainer.sizeParticles(), 1);
    testContainer.removeParticle(p);

}

TEST(LinkedCellContainerTest, correctCellInitialization) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{180,90,1}, 3.0);
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

    s.makeShadowParticle(r);
    EXPECT_EQ(s.getID(), -1);
}