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

/* checks if particle container has the correct number of particles when generating membrane */
TEST(ParticleGeneratorTest, checkMembraneSize) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {2.0, 2.0, 1.0}; // 2x2x1 grid
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    int type = 0;
    double epsilon = 1.0;
    double sigma = 1.0;
    double initTemperature = 0.0;
    bool isFixed = false;
    bool is3d = false;
    double stiffnessConstant = 100.0;

    ParticleGenerator::generateMembrane(pc, cuboid, type, epsilon, sigma, initTemperature, isFixed, is3d, stiffnessConstant);

    // Check particle count: 2x2x1 = 4
    EXPECT_EQ(pc.sizeParticles(), 4);
}

/* checks if the membrane particles are initialized with the correct positions */
TEST(ParticleGeneratorTest, checkMembraneParticlePositions) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {2.0, 2.0, 1.0}; // 2x2x1 grid
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    int type = 0;
    double epsilon = 1.0;
    double sigma = 1.0;
    double initTemperature = 0.0;
    bool isFixed = false;
    bool is3d = false;
    double stiffnessConstant = 100.0;

    ParticleGenerator::generateMembrane(pc, cuboid, type, epsilon, sigma, initTemperature, isFixed, is3d, stiffnessConstant);

    // Expected positions
    std::array<std::array<double, 3>, 4> expected_positions = {
        std::array<double, 3>{0.0, 0.0, 0.0},
        std::array<double, 3>{1.0, 0.0, 0.0},
        std::array<double, 3>{0.0, 1.0, 0.0},
        std::array<double, 3>{1.0, 1.0, 0.0}
    };

    int i = 0;
    for (Particle &p : pc) {
        EXPECT_EQ(p.getX()[0], expected_positions[i][0]);
        EXPECT_EQ(p.getX()[1], expected_positions[i][1]);
        EXPECT_EQ(p.getX()[2], expected_positions[i][2]);
        i++;
    }
}

/* checks if the neighbors are assigned correctly in membrane generation */
TEST(ParticleGeneratorTest, checkMembraneNeighborAssignments) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {2.0, 2.0, 1.0}; // 2x2x1 grid
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    int type = 0;
    double epsilon = 1.0;
    double sigma = 1.0;
    double initTemperature = 0.0;
    bool isFixed = false;
    bool is3d = false;
    double stiffnessConstant = 100.0;

    ParticleGenerator::generateMembrane(pc, cuboid, type, epsilon, sigma, initTemperature, isFixed, is3d, stiffnessConstant);

    // Check neighbor assignments
    for (Particle &particle : pc) {
        for (const auto &neighbor : particle.getNeighbourParticles()) {
            long neighbor_ptr_diff = std::get<0>(neighbor);
            double distance = std::get<1>(neighbor);

            Particle *neighbor_particle = &particle + neighbor_ptr_diff;

            // Verify neighbor distance
            EXPECT_NEAR(distance, ArrayUtils::L2Norm(particle.getX() - neighbor_particle->getX()), 1e-5);
        }
    }
}

/* checks if neighbor relationships are symmetric */
TEST(ParticleGeneratorTest, checkNeighborSymmetry) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {2.0, 2.0, 1.0}; // 2x2x1 grid
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    int type = 0;
    double epsilon = 1.0;
    double sigma = 1.0;
    double initTemperature = 0.0;
    bool isFixed = false;
    bool is3d = false;
    double stiffnessConstant = 100.0;

    ParticleGenerator::generateMembrane(pc, cuboid, type, epsilon, sigma, initTemperature, isFixed, is3d, stiffnessConstant);

    // Check symmetric neighbor relationships
    for (Particle &particle : pc) {
        for (const auto &neighbor : particle.getNeighbourParticles()) {
            long neighbor_ptr_diff = std::get<0>(neighbor);
            Particle *neighbor_particle = &particle + neighbor_ptr_diff;

            bool reverse_found = false;
            for (const auto &reverse_neighbor : neighbor_particle->getNeighbourParticles()) {
                if (&particle == neighbor_particle + std::get<0>(reverse_neighbor)) {
                    reverse_found = true;
                    break;
                }
            }
            EXPECT_TRUE(reverse_found);
        }
    }
}

/* checks if the particle container has the correct number of particles in a 3D membrane */
TEST(ParticleGeneratorTest, check3DMembraneSize) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {2.0, 2.0, 2.0}; // 2x2x2 grid
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    int type = 0;
    double epsilon = 1.0;
    double sigma = 1.0;
    double initTemperature = 0.0;
    bool isFixed = false;
    bool is3d = true; // Set to 3D
    double stiffnessConstant = 100.0;

    ParticleGenerator::generateMembrane(pc, cuboid, type, epsilon, sigma, initTemperature, isFixed, is3d, stiffnessConstant);

    // Check particle count: 2x2x2 = 8
    EXPECT_EQ(pc.sizeParticles(), 8);
}

/* checks if the 3D membrane particles are initialized with the correct positions */
TEST(ParticleGeneratorTest, check3DMembraneParticlePositions) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {2.0, 2.0, 2.0}; // 2x2x2 grid
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    int type = 0;
    double epsilon = 1.0;
    double sigma = 1.0;
    double initTemperature = 0.0;
    bool isFixed = false;
    bool is3d = true; // Set to 3D
    double stiffnessConstant = 100.0;

    ParticleGenerator::generateMembrane(pc, cuboid, type, epsilon, sigma, initTemperature, isFixed, is3d, stiffnessConstant);

    // Expected positions for a 2x2x2 grid
    std::array<std::array<double, 3>, 8> expected_positions = {
        std::array<double, 3>{0.0, 0.0, 0.0}, std::array<double, 3>{1.0, 0.0, 0.0},
        std::array<double, 3>{0.0, 1.0, 0.0}, std::array<double, 3>{1.0, 1.0, 0.0},
        std::array<double, 3>{0.0, 0.0, 1.0}, std::array<double, 3>{1.0, 0.0, 1.0},
        std::array<double, 3>{0.0, 1.0, 1.0}, std::array<double, 3>{1.0, 1.0, 1.0}
    };

    int i = 0;
    for (Particle &p : pc) {
        EXPECT_EQ(p.getX()[0], expected_positions[i][0]);
        EXPECT_EQ(p.getX()[1], expected_positions[i][1]);
        EXPECT_EQ(p.getX()[2], expected_positions[i][2]);
        i++;
    }
}

/* checks if neighbors are correctly assigned in a 3D membrane */
TEST(ParticleGeneratorTest, check3DMembraneNeighborAssignments) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {2.0, 2.0, 2.0}; // 2x2x2 grid
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    int type = 0;
    double epsilon = 1.0;
    double sigma = 1.0;
    double initTemperature = 0.0;
    bool isFixed = false;
    bool is3d = true; // Set to 3D
    double stiffnessConstant = 100.0;

    ParticleGenerator::generateMembrane(pc, cuboid, type, epsilon, sigma, initTemperature, isFixed, is3d, stiffnessConstant);

    // Verify neighbors for all particles
    for (Particle &particle : pc) {
        for (const auto &neighbor : particle.getNeighbourParticles()) {
            long neighbor_ptr_diff = std::get<0>(neighbor);
            double distance = std::get<1>(neighbor);

            Particle *neighbor_particle = &particle + neighbor_ptr_diff;

            // Verify neighbor distance
            EXPECT_NEAR(distance, ArrayUtils::L2Norm(particle.getX() - neighbor_particle->getX()), 1e-5);
        }
    }
}

/* checks neighbor symmetry in a 3D membrane */
TEST(ParticleGeneratorTest, check3DMembraneNeighborSymmetry) {
    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> N = {2.0, 2.0, 2.0}; // 2x2x2 grid
    std::array<double, 3> V = {0.0, 0.0, 0.0};
    double h = 1.0;
    double m = 1.0;
    double mv = 0.0;
    Cuboid cuboid(x, N, h, m, V, mv);
    ParticleContainers::DirectSumContainer pc;

    int type = 0;
    double epsilon = 1.0;
    double sigma = 1.0;
    double initTemperature = 0.0;
    bool isFixed = false;
    bool is3d = true; // Set to 3D
    double stiffnessConstant = 100.0;

    ParticleGenerator::generateMembrane(pc, cuboid, type, epsilon, sigma, initTemperature, isFixed, is3d, stiffnessConstant);

    // Check neighbor symmetry
    for (Particle &particle : pc) {
        for (const auto &neighbor : particle.getNeighbourParticles()) {
            long neighbor_ptr_diff = std::get<0>(neighbor);
            Particle *neighbor_particle = &particle + neighbor_ptr_diff;

            bool reverse_found = false;
            for (const auto &reverse_neighbor : neighbor_particle->getNeighbourParticles()) {
                if (&particle == neighbor_particle + std::get<0>(reverse_neighbor)) {
                    reverse_found = true;
                    break;
                }
            }
            EXPECT_TRUE(reverse_found);
        }
    }
}