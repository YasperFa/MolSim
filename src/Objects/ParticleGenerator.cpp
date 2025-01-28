//
// Created by Yasmine Farah on 10/11/2024.
//

#include "ParticleGenerator.h"

#include "Containers/ParticleContainer.h"
#include "spdlog/spdlog.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include "utils/ArrayUtils.h"


void ParticleGenerator::generateCuboid(ParticleContainers::ParticleContainer &particles, Cuboid &cuboid, int type,
                                       double epsilon, double sigma, double initTemperature, bool isFixed, bool is3d) {
    // iterate over the specified dimensions and generate particles
    SPDLOG_DEBUG("generating cuboid particles");
    std::array<double, 3> N = cuboid.getNumOfParticlesPerDimension();
    std::array<double, 3> x = cuboid.getLowerFrontCorner();
    std::array<double, 3> v = cuboid.getInitVelocity();
    double h = cuboid.getDistBetweenParticles();
    double m = cuboid.getMass();
    double mv = cuboid.getMeanVelocity();

    int dimensions = is3d ? 3 : 2;

    for (int k = 0; k < N[2]; ++k) {
        for (int j = 0; j < N[1]; ++j) {
            for (int i = 0; i < N[0]; ++i) {
                // calculate x,y,z coordinates
                std::array<double, 3> particle_pos = {
                    x[0] + i * h,
                    x[1] + j * h,
                    x[2] + k * h
                };
                // get the maxwell velocity
                std::array<double, 3> maxwell_vel = maxwellBoltzmannDistributedVelocity(mv, dimensions);
                // get the initial velocity
                std::array<double, 3> vel = v;
                //scale according to temperature

                if (initTemperature != -1) {
                    double scale = 1;
                    scale = std::sqrt(initTemperature / m);
                    maxwell_vel = maxwellBoltzmannDistributedVelocity(scale, dimensions);
                }
                // add maxwell velocity to initial velocity
                for (int d = 0; d < 3; ++d) {
                    vel[d] += maxwell_vel[d];
                }

                auto newParticle = std::make_shared<Particle>(particle_pos, vel, m, type, epsilon, sigma, isFixed);
                particles.addParticle(*newParticle);
            }
        }
    }
}

void ParticleGenerator::generateMembrane(ParticleContainers::ParticleContainer &particles, Cuboid &cuboid, int type,
                                         double epsilon, double sigma, double initTemperature, bool isFixed, bool is3d,
                                         double stiffnessConstant) {
    // Step 1: Generate the particle grid using the existing generateCuboid function
    generateCuboid(particles, cuboid, type, epsilon, sigma, initTemperature, isFixed, is3d);

    // Step 2: Extract parameters for neighbor assignment
    std::array<double, 3> grid_dimensions = cuboid.getNumOfParticlesPerDimension();

    // Step 3: Iterate through particles to assign neighbors
    auto particle_it = particles.begin();
    for (int i = 0; i < static_cast<int>(grid_dimensions[0]); i++) {
        for (int j = 0; j < static_cast<int>(grid_dimensions[1]); j++) {
            for (int k = 0; k < static_cast<int>(grid_dimensions[2]); k++) {
                Particle &current_particle = *particle_it;

                // Assign neighbors using offsets
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dz = -1; dz <= 1; dz++) {
                            if (dx == 0 && dy == 0 && dz == 0) continue;

                            int ni = i + dx;
                            int nj = j + dy;
                            int nk = k + dz;

                            // Ensure neighbors are within bounds
                            if (ni < 0 || ni >= static_cast<int>(grid_dimensions[0]) || nj < 0 ||
                                nj >= static_cast<int>(grid_dimensions[1]) || nk < 0 ||
                                nk >= static_cast<int>(grid_dimensions[2])) {
                                continue;
                            }

                            // Calculate linear index of neighbor
                            int neighbor_index =
                                    ni * static_cast<int>(grid_dimensions[1]) * static_cast<int>(grid_dimensions[2]) +
                                    nj * static_cast<int>(grid_dimensions[2]) + nk;

                            auto neighbor_it = particles.begin();
                            std::advance(neighbor_it, neighbor_index);

                            Particle &neighbor_particle = *neighbor_it;

                            // Calculate initial distance and add connection
                            double initial_distance = ArrayUtils::L2Norm(
                                current_particle.getX() - neighbor_particle.getX());
                            current_particle.addNeighbourParticle(&neighbor_particle - &current_particle,
                                                                  initial_distance, stiffnessConstant);
                        }
                    }
                }

                ++particle_it;
            }
        }
    }
}

void ParticleGenerator::generateDisc(ParticleContainers::ParticleContainer &particles, Disc &disc, int type,
                                     double epsilon, double sigma, double initTemperature, bool isFixed, bool is3d) {
    const std::array<double, 3> center = disc.getCenterCoordinate();
    std::array<double, 3> initVel = disc.getInitVelocity();
    const int r = disc.getRadius();
    const double mass = disc.getMass();
    const double h = disc.getDistanceBetweenParticles();
    int dimensions = is3d ? 3 : 2;

    if(!is3d) {
    SPDLOG_DEBUG("generating disc particles for 2d");
    SPDLOG_DEBUG("r: {}", r);
    for (int i = -r; i <= r; ++i) {
        SPDLOG_DEBUG("column {}", i);
        // check if position is inside the circle j*j + i*i <= r*r
        for (int j = -r; j <= r; ++j) {
            if (((j * j) + (i * i) <= r * r)) {
                if (initTemperature != -1) {
                    double scale = std::sqrt(initTemperature / mass);
                    std::array<double, 3> maxwell_vel = maxwellBoltzmannDistributedVelocity(scale, dimensions);
                    for (int m = 0; m < 3; ++m) {
                        initVel[m] += maxwell_vel[m];
                    }
                }
                SPDLOG_DEBUG("line {}", j);
                const std::array<double, 3> particlePosition = {center[0] + j * h, center[1] + i * h, center[2]};
                // create new particle
                Particle nParticle(particlePosition, initVel, mass, type, epsilon, sigma, isFixed);
                // add new particle to container
                particles.addParticle(nParticle);
            }
        }
    }
    } else {
        SPDLOG_DEBUG("generating disc particles for 3d");
         for (int i = -r; i <= r; ++i) {
        SPDLOG_DEBUG("column {}", i);
        // check if position is inside the circle j*j + i*i + k*k<= r*r
        for (int j = -r; j <= r; ++j) {
            for (int k = -r; k <= r; k++){

            if (((j * j) + (i * i) + (k * k) <= r * r)) {
                if (initTemperature != -1) {
                    double scale = std::sqrt(initTemperature / mass);
                    std::array<double, 3> maxwell_vel = maxwellBoltzmannDistributedVelocity(scale, dimensions);
                    for (int m = 0; m < 3; ++m) {
                        initVel[m] += maxwell_vel[m];
                    }
                }
                SPDLOG_DEBUG("line {}", j);
                const std::array<double, 3> particlePosition = {center[0] + j * h, center[1] + i * h, center[2] + k * h};
                SPDLOG_DEBUG("{} {} {}", center[0] + j * h, center[1] + i * h, center[2] + k * h);
                // create new particle
                Particle nParticle(particlePosition, initVel, mass, type, epsilon, sigma, isFixed);
                // add new particle to container
                particles.addParticle(nParticle);
            }
            }
        }
    }

    }
}
