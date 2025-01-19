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
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {1.0, 1.0, 1.0};
    std::array<double, 3> V = {2.0, 0.0, 0.0};
    double h = 1.0;
    double m = 0.5;
    double mv = 0.5;
    Cuboid cuboid1(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateCuboid(pc, cuboid1, 0, 5, 1, -1, false);
    // size should be 1*1*1
    EXPECT_EQ(1, pc.sizeParticles());
    N = {2.0, 1.0, 1.0};
    Cuboid cuboid2(x, N, h, m, V, mv);
    ParticleGenerator::generateCuboid(pc, cuboid2, 0, 5, 1, -1, false);
    // size should be 2*1*1 plus existing particles
    EXPECT_EQ(3, pc.sizeParticles());
    N = {2.0, 4.0, 3.0};
    Cuboid cuboid3(x, N, h, m, V, mv);
    ParticleGenerator::generateCuboid(pc, cuboid3, 0, 5, 1, -1, false);
    // size should be 2*4*3 plus existing particles
    EXPECT_EQ(27, pc.sizeParticles());
}

/* checks if particle container has the correct number of particles when generating disc*/
TEST(ParticleGeneratorTest, checkContainerSize2) {
    std::array<double, 3> center = {1.0, 1.0, 1.0};
    std::array<double, 3> initVel = {0.0, 1.0, 2.0};
    int r = 0;
    double mass = 0.5;
    double h = 1.0;
    Disc disc(center, initVel, r, h, mass);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateDisc(pc, disc, 0, 5, 1, false);
    // size should be 1
    EXPECT_EQ(1, pc.sizeParticles());
    r = 1;
    Disc disc2(center, initVel, r, mass, h);
    ParticleGenerator::generateDisc(pc, disc2, 0, 5, 1, false);
    // size should be 1 + 5
    EXPECT_EQ(6, pc.sizeParticles());
    r = 2;
    Disc disc3(center, initVel, r, mass, h);
    ParticleGenerator::generateDisc(pc, disc3, 0, 5, 1, false);
    // size should be 1 + 5 + 13
    EXPECT_EQ(19, pc.sizeParticles());
}

/* checks if the particles are initialized with the correct position for cuboids */
TEST(ParticleGeneratorTest, checkParticlePositions1) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {2.0, 2.0, 2.0};
    std::array<double, 3> V = {2.0, 0.0, 0.0};
    double h = 0.5;
    double m = 0.5;
    double mv = 0.5;
    Cuboid cuboid1(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateCuboid(pc, cuboid1, 0, 5, 1, -1, false);
    std::array<std::array<double, 3>, 8> expected = {
        std::array<double, 3>{0.0, 0.0, 0.0}, std::array<double, 3>{0.5, 0.0, 0.0},
        std::array<double, 3>{0.0, 0.5, 0.0}, std::array<double, 3>{0.5, 0.5, 0.0},
        std::array<double, 3>{0.0, 0.0, 0.5}, std::array<double, 3>{0.5, 0.0, 0.5},
        std::array<double, 3>{0.0, 0.5, 0.5}, std::array<double, 3>{0.5, 0.5, 0.5}
    };
    int i = 0;
    for (Particle &p: pc) {
        EXPECT_EQ(expected[i][0], p.getX()[0]);
        EXPECT_EQ(expected[i][1], p.getX()[1]);
        EXPECT_EQ(expected[i][2], p.getX()[2]);
        i++;
    }
}

/* checks if the particles are initialized with the correct position for discs*/
TEST(ParticleGeneratorTest, checkParticlePositions2) {
    std::array<double, 3> center = {1.0, 1.0, 0.0};
    std::array<double, 3> initVel = {0.0, 1.0, 2.0};
    int r = 1;
    double mass = 0.5;
    double h = 1.0;
    Disc disc(center, initVel, r, h, mass);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateDisc(pc, disc, 0, 5, 1, -1);
    std::array<std::array<double, 3>, 5> expected = {
        std::array<double, 3>{1.0, 0.0, 0.0}, std::array<double, 3>{0.0, 1.0, 0.0},
        std::array<double, 3>{1.0, 1.0, 0.0}, std::array<double, 3>{2.0, 1.0, 0.0},
        std::array<double, 3>{1.0, 2.0, 0.0}
    };
    int i = 0;
    for (Particle &p: pc) {
        EXPECT_EQ(expected[i][0], p.getX()[0]);
        EXPECT_EQ(expected[i][1], p.getX()[1]);
        EXPECT_EQ(expected[i][2], p.getX()[2]);
        i++;
    }
}

/* checks if the generator correctly initializes mass, left up front corner*/
TEST(ParticleGeneratorTest, checkOtherVariables) {
    std::array<double, 3> x = {0.7, 0.0, 0.0};
    std::array<double, 3> N = {1.0, 1.0, 1.0};
    std::array<double, 3> V = {2.0, 0.0, 3.0};
    double h = 5.0;
    double m = 0.5;
    double mv = 1.0;
    Cuboid cuboid1(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;
    ParticleGenerator::generateCuboid(pc, cuboid1, 0, 5, 1, -1, false);
    for (Particle &p: pc) {
        //check X
        EXPECT_EQ(0.7, p.getX()[0]);
        EXPECT_EQ(0.0, p.getX()[1]);
        EXPECT_EQ(0.0, p.getX()[2]);
        //check m
        EXPECT_EQ(0.5, p.getM());
    }
}

/* Test if direct neighbors are correctly assigned for a small cuboid */
TEST(ParticleGeneratorTest, DirectNeighborAssignment) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {3.0, 3.0, 1.0}; // 3x3 grid in 2D
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    // Generate particles with neighbor assignment
    ParticleGenerator::generateCuboid(pc, cuboid, 0, 1.0, 1.0, -1.0, false, true, false);

    const auto &particles = pc.getParticles();

    // Check neighbors for the center particle (1, 1, 0)
    auto &centerParticle = particles[4]; // 1D index: 1 + 1 * 3 = 4
    EXPECT_EQ(centerParticle.getDirectNeighbourIds().size(), 4); // Direct neighbors in 2D
    EXPECT_EQ(centerParticle.getDiagonalNeighbourIds().size(), 4); // Diagonal neighbors in 2D

    // Verify a specific neighbor (direct neighbor below)
    EXPECT_TRUE(std::find(centerParticle.getDirectNeighbourIds().begin(),
        centerParticle.getDirectNeighbourIds().end(),
        particles[1].getID()) != centerParticle.getDirectNeighbourIds().end());
}

/* Test if diagonal neighbors are correctly assigned for a small cuboid */
TEST(ParticleGeneratorTest, DiagonalNeighborAssignment) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {3.0, 3.0, 3.0}; // 3x3x3 grid in 3D
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    // Generate particles with neighbor assignment
    ParticleGenerator::generateCuboid(pc, cuboid, 0, 1.0, 1.0, -1.0, false, true, true);

    const auto &particles = pc.getParticles();

    // Check neighbors for the center particle in a 3D grid
    auto &centerParticle = particles[13];
    EXPECT_EQ(centerParticle.getDirectNeighbourIds().size(), 6); // Direct neighbors in 3D
    EXPECT_EQ(centerParticle.getDiagonalNeighbourIds().size(), 20); // Diagonal neighbors in 3D

    // Verify a specific diagonal neighbor
    EXPECT_TRUE(std::find(centerParticle.getDiagonalNeighbourIds().begin(),
        centerParticle.getDiagonalNeighbourIds().end(),
        particles[0].getID()) != centerParticle.getDiagonalNeighbourIds().end());
}

/* Test if corner particles have fewer neighbors */
TEST(ParticleGeneratorTest, CornerParticleNeighborAssignment) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {3.0, 3.0, 3.0}; // 3x3x3 grid in 3D
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    // Generate particles with neighbor assignment
    ParticleGenerator::generateCuboid(pc, cuboid, 0, 1.0, 1.0, -1.0, false, true, true);

    const auto &particles = pc.getParticles();

    // Check neighbors for a corner particle (0, 0, 0)
    auto &cornerParticle = particles[0];
    EXPECT_EQ(cornerParticle.getDirectNeighbourIds().size(), 3); // 3 direct neighbors for corner in 3D
    EXPECT_EQ(cornerParticle.getDiagonalNeighbourIds().size(), 4); // 4 diagonal neighbors for corner in 3D
}

/* Test neighbor assignment for a 2D cuboid */
TEST(ParticleGeneratorTest, TwoDimensionalNeighborAssignment) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {3.0, 3.0, 1.0}; // 3x3 grid in 2D
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    // Generate particles with neighbor assignment
    ParticleGenerator::generateCuboid(pc, cuboid, 0, 1.0, 1.0, -1.0, false, true, false);

    const auto &particles = pc.getParticles();

    // Check neighbors for a boundary particle (1, 0, 0)
    auto &boundaryParticle = particles[1]; // 1D index for (1, 0, 0): 1
    EXPECT_EQ(boundaryParticle.getDirectNeighbourIds().size(), 3); // Boundary in 2D
    EXPECT_EQ(boundaryParticle.getDiagonalNeighbourIds().size(), 2); // Diagonal in 2D
}
