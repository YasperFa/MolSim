//
// Created by Yasmine Farah on 12/11/2024.
//

#include <gtest/gtest.h>
#include "../../src/Objects/ParticleGenerator.h"
#include "../../src/Objects/Cuboid.h"
#include "../../src/Objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
TEST(ParticleGeneratorTest, checkContainerSize) {
    std::array<double,3> x = {0.0,0.0,0.0} ;
    std::array<double,3> N = {1.0, 1.0, 1.0} ;
    std::array<double,3> V = {2.0, 0.0, 0.0} ;
    double h = 1.0 ;
    double m = 0.5 ;
    double mv = 0.5 ;
    Cuboid cuboid1(x,N, h, m,V,mv);
    ParticleContainer pc;
    ParticleGenerator::generateCuboid(pc,cuboid1);
    // size should be 1*1*1
    EXPECT_EQ(1,pc.sizeParticles());
    N = {2.0, 1.0, 1.0};
    Cuboid cuboid2(x,N, h, m,V,mv);
    ParticleGenerator::generateCuboid(pc,cuboid2);
    // size should be 2*1*1 plus existing particles
    EXPECT_EQ(3,pc.sizeParticles());
    N = {2.0, 4.0, 3.0};
    Cuboid cuboid3(x,N, h, m,V,mv);
    ParticleGenerator::generateCuboid(pc,cuboid3);
    // size should be 2*4*3 plus existing particles
    EXPECT_EQ(27,pc.sizeParticles());
}
TEST(ParticleGeneratorTest, checkParticlePositions) {
    std::array<double,3> x = {0.0,0.0,0.0} ;
    std::array<double,3> N = {2.0, 2.0, 2.0} ;
    std::array<double,3> V = {2.0, 0.0, 0.0} ;
    double h = 0.5 ;
    double m = 0.5 ;
    double mv = 0.5 ;
    Cuboid cuboid1(x,N, h, m,V,mv);
    ParticleContainer pc;
    ParticleGenerator::generateCuboid(pc,cuboid1);
    std::array<std::array<double,3>,8> expected = {std::array<double,3>{0.0,0.0,0.0},std::array<double,3>{0.5,0.0,0.0},std::array<double,3>{0.0,0.5,0.0},std::array<double,3>{0.5,0.5,0.0},std::array<double,3>{0.0,0.0,0.5},std::array<double,3>{0.5,0.0,0.5},std::array<double,3>{0.0,0.5,0.5},std::array<double,3>{0.5,0.5,0.5}};
    int i = 0;
    for (Particle &p : pc) {
        EXPECT_EQ(expected[i][0] , p.getX()[0]);
        EXPECT_EQ(expected[i][1] , p.getX()[1]);
        EXPECT_EQ(expected[i][2] , p.getX()[2]);
        i++;
    }


}
TEST(ParticleGeneratorTest, checkOtherVariables) {
    std::array<double,3> x = {0.7,0.0,0.0} ;
    std::array<double,3> N = {1.0, 1.0, 1.0} ;
    std::array<double,3> V = {2.0, 0.0, 3.0} ;
    double h = 5.0 ;
    double m = 0.5 ;
    double mv = 1.0 ;
    Cuboid cuboid1(x,N, h, m,V,mv);
    ParticleContainer pc;
    ParticleGenerator::generateCuboid(pc,cuboid1);
    for (Particle &p : pc) {
        //check X
        EXPECT_EQ(0.7 , p.getX()[0]);
        EXPECT_EQ(0.0 , p.getX()[1]);
        EXPECT_EQ(0.0 , p.getX()[2]);
        //check m
        EXPECT_EQ(0.5 , p.getM());

    }

}
