//
// Created by Yasmine Farah on 01/11/2024.
//
#include "gtest/gtest.h"
#include "../src/ParticleContainer.h"
#include "../src/Particle.h"

// check if sizeParticles() works correctly
TEST(ParticleContainerTest, StrctureAfterAddParticle) {
    ParticleContainer test;
    Particle p(0);
    test.addParticle(p);
    EXPECT_EQ(test.sizeParticles(), 1);

}
