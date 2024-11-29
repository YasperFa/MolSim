//
// Created by Yasmine Farah on 01/11/2024.
//
#include "gtest/gtest.h"
#include "../../src/Objects/Containers/DirectSum/DirectSumContainer.h"
#include "../src/Objects/Particle.h"

/* Checks if sizeParticles() and addParticle() works correctly */
TEST(DirectSumContainerTest, StrctureAfterAddParticle) {
    ParticleContainers::DirectSumContainer test;
    Particle p(0);
    
    EXPECT_EQ(test.sizeParticles(), 0);
    test.addParticle(p);
    EXPECT_EQ(test.sizeParticles(), 1);

    test.removeParticle(p);
    EXPECT_EQ(test.sizeParticles(), 0);
    

}


/* Checks if addParticleToPairs() and removeParticle() work correctly*/
/*TEST(ParticleContainerTest, addParticleToPairs) {
    ParticleContainer test;
    
    Particle p(0);
    test.addParticleToPairs(p);
    EXPECT_EQ(test.getParticlePairs().size(), 0);
    EXPECT_EQ(test.getParticles().size(), 1);
    EXPECT_NO_THROW(test.getParticle(p));

    Particle q(0);
    test.addParticleToPairs(q);
    EXPECT_EQ(test.getParticlePairs().size(), 1);
    EXPECT_EQ(test.getParticles().size(), 2);
    EXPECT_NO_THROW(test.getParticle(q));

    Particle r(0);
    test.addParticleToPairs(r);

    EXPECT_EQ(test.getParticlePairs().size(), 3);
    EXPECT_EQ(test.getParticles().size(), 3);
    EXPECT_NO_THROW(test.getParticle(r));

    Particle s(0);
    test.addParticleToPairs(s);
    EXPECT_EQ(test.getParticlePairs().size(), 6);
    EXPECT_EQ(test.getParticles().size(), 4);
    EXPECT_NO_THROW(test.getParticle(s));

    test.removeParticle(r);
    EXPECT_EQ(test.getParticlePairs().size(), 3);
    EXPECT_EQ(test.getParticles().size(), 3);
    EXPECT_ANY_THROW(test.getParticle(r));

    test.removeParticle(p);
    EXPECT_EQ(test.getParticlePairs().size(), 1);
    EXPECT_EQ(test.getParticles().size(), 2);
    EXPECT_ANY_THROW(test.getParticle(p));

    test.removeParticle(s);
    EXPECT_EQ(test.getParticlePairs().size(), 0);
    EXPECT_EQ(test.getParticles().size(), 1);
    EXPECT_ANY_THROW(test.getParticle(s));

    test.removeParticle(q);
    EXPECT_EQ(test.getParticlePairs().size(), 0);
    EXPECT_EQ(test.getParticles().size(), 0);
    EXPECT_ANY_THROW(test.getParticle(q));

}*/
