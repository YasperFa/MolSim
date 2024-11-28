//
// Created by Yasmine Farah on 01/11/2024.
//
#include "gtest/gtest.h"
#include "../../src/Objects/Containers/DirectSum/DirectSumContainer.h"
#include "../src/Objects/Particle.h"

/** Checks if sizeParticles() and addParticle() works correctly */
TEST(ParticleContainerTest, StrctureAfterAddParticle) {
    DirectSumContainer testContainer;
    Particle p(0);
    
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    testContainer.addParticle(p);
    EXPECT_EQ(testContainer.sizeParticles(), 1);

    testContainer.removeParticle(p);
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    

}


/** Checks if addParticle() and removeParticle() work correctly*/
TEST(ParticleContainerTest, addParticle) {
    DirectSumContainer testContainer;
    
    Particle p(0);
    testContainer.addParticle(p);
    
    EXPECT_EQ(testContainer.getParticles().size(), 1);
    EXPECT_NO_THROW(testContainer.getParticle(p));

    Particle q(0);
    testContainer.addParticle(q);
    EXPECT_EQ(testContainer.getParticles().size(), 2);
    EXPECT_NO_THROW(testContainer.getParticle(q));

    Particle r(0);
    testContainer.addParticle(r);
    EXPECT_EQ(testContainer.getParticles().size(), 3);
    EXPECT_NO_THROW(testContainer.getParticle(r));

    Particle s(0);
    testContainer.addParticle(s);
    EXPECT_EQ(testContainer.getParticles().size(), 4);
    EXPECT_NO_THROW(testContainer.getParticle(s));

    testContainer.removeParticle(r);
    EXPECT_EQ(testContainer.getParticles().size(), 3);
    EXPECT_ANY_THROW(testContainer.getParticle(r));

    testContainer.removeParticle(p);
    EXPECT_EQ(testContainer.getParticles().size(), 2);
    EXPECT_ANY_THROW(testContainer.getParticle(p));

    testContainer.removeParticle(s);
    
    EXPECT_EQ(testContainer.getParticles().size(), 1);
    EXPECT_ANY_THROW(testContainer.getParticle(s));

    testContainer.removeParticle(q);
    
    EXPECT_EQ(testContainer.getParticles().size(), 0);
    EXPECT_ANY_THROW(testContainer.getParticle(q));

}

/** Tests that ids of new particles get initialized correctly */
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

    s.makeShadowParticle();
    EXPECT_EQ(s.getID(), -3);
}