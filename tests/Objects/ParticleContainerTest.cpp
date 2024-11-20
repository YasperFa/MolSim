//
// Created by Yasmine Farah on 01/11/2024.
//
#include "gtest/gtest.h"
#include "../src/Objects/ParticleContainer.h"
#include "../src/Objects/Particle.h"

/* Checks if sizeParticles() and addParticle() works correctly */
TEST(ParticleContainerTest, StrctureAfterAddParticle) {
    ParticleContainer test;
    Particle p(0);
    EXPECT_EQ(test.sizeParticles(), 0);
    test.addParticle(p);
    EXPECT_EQ(test.sizeParticles(), 1);
    test.removeParticle(p);
    EXPECT_EQ(test.sizeParticles(), 0);
    

}

/* Checks if pairs are initialized correctly*/
TEST(ParticleContainerTest, PairsAfterAddParticle) {
    ParticleContainer test;
    Particle p(0);
    Particle q(1);
    test.addParticle(p);
    test.addParticle(q);

    test.initializePairsVector();
    EXPECT_EQ(test.getParticlePairs().size(), 1);

    Particle r(1);
    test.addParticle(r);
    test.reinitializePairs();
    EXPECT_EQ(test.getParticlePairs().size(), 3);

    Particle s(1);
    test.addParticle(s);
    test.reinitializePairs();
    EXPECT_EQ(test.getParticlePairs().size(), 6);

    test.removeParticle(s);
    test.reinitializePairs();
    EXPECT_EQ(test.getParticlePairs().size(), 3);

}
