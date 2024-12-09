//
// Created by Yasmine Farah on 12/11/2024.
//

#include <gtest/gtest.h>
#include "../../src/Objects/ParticleGenerator.h"
#include "../../src/Objects/Shapes/Cuboid.h"
#include "../../src/Objects/Containers/DirectSum/DirectSumContainer.h"
#include "utils/ArrayUtils.h"


/* checks if particle container has the correct number of particles when generating cuboid */
TEST(ParticleGeneratorTest, checkContainerSize1) {
    std::array<double,3> x = {0.0,0.0,0.0} ;
    std::array<double,3> N = {1.0, 1.0, 1.0} ;
    std::array<double,3> V = {2.0, 0.0, 0.0} ;
    double h = 1.0 ;
    double m = 0.5 ;
    double mv = 0.5 ;
    Cuboid cuboid1(x,N, h, m,V,mv);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateCuboid(pc,cuboid1, 0, 5, 1);
    // size should be 1*1*1
    EXPECT_EQ(1,pc.sizeParticles());
    N = {2.0, 1.0, 1.0};
    Cuboid cuboid2(x,N, h, m,V,mv);
    ParticleGenerator::generateCuboid(pc,cuboid2, 0, 5, 1);
    // size should be 2*1*1 plus existing particles
    EXPECT_EQ(3,pc.sizeParticles());
    N = {2.0, 4.0, 3.0};
    Cuboid cuboid3(x,N, h, m,V,mv);
    ParticleGenerator::generateCuboid(pc,cuboid3, 0, 5, 1);
    // size should be 2*4*3 plus existing particles
    EXPECT_EQ(27,pc.sizeParticles());
}
/* checks if particle container has the correct number of particles when generating disc*/
TEST(ParticleGeneratorTest, checkContainerSize2) {
    std::array<double, 3> center = {1.0,1.0,1.0};
    std::array<double, 3> initVel = {0.0,1.0,2.0};
    int r = 0;
    double mass = 0.5;
    double h = 1.0;
    Disc disc(center,initVel,r,h,mass);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateDisc(pc,disc, 0, 5, 1);
    // size should be 1
    EXPECT_EQ(1,pc.sizeParticles());
    r = 1;
    Disc disc2(center,initVel,r,mass,h);
    ParticleGenerator::generateDisc(pc,disc2, 0, 5, 1);
    // size should be 1 + 5
    EXPECT_EQ(6,pc.sizeParticles());
    r = 2;
    Disc disc3(center,initVel,r,mass,h);
    ParticleGenerator::generateDisc(pc,disc3, 0, 5, 1);
    // size should be 1 + 5 + 13
    EXPECT_EQ(19 ,pc.sizeParticles());

}
/* checks if the particles are initialized with the correct position for cuboids */
TEST(ParticleGeneratorTest, checkParticlePositions1) {
    std::array<double,3> x = {0.0,0.0,0.0} ;
    std::array<double,3> N = {2.0, 2.0, 2.0} ;
    std::array<double,3> V = {2.0, 0.0, 0.0} ;
    double h = 0.5 ;
    double m = 0.5 ;
    double mv = 0.5 ;
    Cuboid cuboid1(x,N, h, m,V,mv);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateCuboid(pc,cuboid1, 0, 5, 1);
    std::array<std::array<double,3>,8> expected = {std::array<double,3>{0.0,0.0,0.0},std::array<double,3>{0.5,0.0,0.0},std::array<double,3>{0.0,0.5,0.0},std::array<double,3>{0.5,0.5,0.0},std::array<double,3>{0.0,0.0,0.5},std::array<double,3>{0.5,0.0,0.5},std::array<double,3>{0.0,0.5,0.5},std::array<double,3>{0.5,0.5,0.5}};
    int i = 0;
    for (Particle &p : pc) {
        EXPECT_EQ(expected[i][0] , p.getX()[0]);
        EXPECT_EQ(expected[i][1] , p.getX()[1]);
        EXPECT_EQ(expected[i][2] , p.getX()[2]);
        i++;
    }
}
/* checks if the particles are initialized with the correct position for discs*/
TEST(ParticleGeneratorTest, checkParticlePositions2) {
    std::array<double, 3> center = {1.0,1.0,0.0};
    std::array<double, 3> initVel = {0.0,1.0,2.0};
    int r = 1;
    double mass = 0.5;
    double h = 1.0;
    Disc disc(center,initVel,r,h,mass);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateDisc(pc,disc, 0, 5, 1);
    std::array<std::array<double,3>,5> expected = {std::array<double,3>{1.0,0.0,0.0},std::array<double,3>{0.0,1.0,0.0},std::array<double,3>{1.0,1.0,0.0},std::array<double,3>{2.0,1.0,0.0},
        std::array<double,3>{1.0,2.0,0.0}};
    int i = 0;
    for (Particle &p : pc) {
        EXPECT_EQ(expected[i][0] , p.getX()[0]);
        EXPECT_EQ(expected[i][1] , p.getX()[1]);
        EXPECT_EQ(expected[i][2] , p.getX()[2]);
        i++;
    }


}
/* checks if the generator correctly initializes mass, left up front corner*/
TEST(ParticleGeneratorTest, checkOtherVariables) {
    std::array<double,3> x = {0.7,0.0,0.0} ;
    std::array<double,3> N = {1.0, 1.0, 1.0} ;
    std::array<double,3> V = {2.0, 0.0, 3.0} ;
    double h = 5.0 ;
    double m = 0.5 ;
    double mv = 1.0 ;
    Cuboid cuboid1(x,N, h, m,V,mv);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateCuboid(pc,cuboid1, 0, 5, 1);
    for (Particle &p : pc) {
        //check X
        EXPECT_EQ(0.7 , p.getX()[0]);
        EXPECT_EQ(0.0 , p.getX()[1]);
        EXPECT_EQ(0.0 , p.getX()[2]);
        //check m
        EXPECT_EQ(0.5 , p.getM());

    }

}