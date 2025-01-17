//
// Created by Yasmine Farah on 10/11/2024.
//

#include "ParticleGenerator.h"

#include "Containers/ParticleContainer.h"
#include "spdlog/spdlog.h"
#include "utils/MaxwellBoltzmannDistribution.h"


void ParticleGenerator::generateCuboid(ParticleContainers::ParticleContainer &particles, Cuboid &cuboid, int type,
                                       double epsilon, double sigma, double initTemperature, bool isFixed,
                                       bool assignNeighbours, bool is3d) {
    // iterate over the specified dimensions and generate particles
    SPDLOG_DEBUG("generating cuboid particles");
    std::array<double, 3> N = cuboid.getNumOfParticlesPerDimension();
    std::array<double, 3> x = cuboid.getLowerFrontCorner();
    std::array<double, 3> v = cuboid.getInitVelocity();
    double h = cuboid.getDistBetweenParticles();
    double m = cuboid.getMass();
    double mv = cuboid.getMeanVelocity();

    std::vector<std::vector<std::vector<std::shared_ptr<Particle> > > > grid(
        static_cast<size_t>(N[0]),
        std::vector<std::vector<std::shared_ptr<Particle> > >(static_cast<size_t>(N[1]),
                                                              std::vector<std::shared_ptr<Particle> >(
                                                                  static_cast<size_t>(N[2]), nullptr)
        )
    );

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
                for (int m = 0; m < 3; ++m) {
                    vel[m] += maxwell_vel[m];
                }

                auto newParticle = std::make_shared<Particle>(particle_pos, vel, m, type, epsilon, sigma, isFixed);
                particles.addParticle(*newParticle);
                grid[i][j][k] = newParticle;
            }
        }
    }

    if (assignNeighbours) {
        static const std::array<std::array<int, 3>, 26> neighbourOffsets = {
            {
                {-1, 0, 0}, {1, 0, 0}, // direct neighbours on the x-axis
                {0, -1, 0}, {0, 1, 0}, // direct neighbours on the y-axis
                {0, 0, -1}, {0, 0, 1}, // direct neighbours on the z-axis
                {-1, -1, 0}, {-1, 1, 0}, {1, -1, 0}, {1, 1, 0}, // diagonal neighbours on the XY plane
                {-1, 0, -1}, {-1, 0, 1}, {1, 0, -1}, {1, 0, 1}, // diagonal neighbours on the XZ plane
                {0, -1, -1}, {0, -1, 1}, {0, 1, -1}, {0, 1, 1}, // diagonal neighbours on the YZ plane
                {-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1}, {1, -1, -1}, {1, -1, 1}, {1, 1, -1},
                {1, 1, 1} // 3D diagonal
            }
        };

        for (int k = 0; k < N[2]; ++k) {
            for (int j = 0; j < N[1]; ++j) {
                for (int i = 0; i < N[0]; ++i) {
                    Particle *current = grid[i][j][k].get();
                    if (!current) {
                        SPDLOG_ERROR("Null particle pointer at grid[{}][{}][{}]", i, j, k);
                        continue; // Skip null pointers
                    }

                    for (const auto &offset: neighbourOffsets) {
                        int ni = i + offset[0];
                        int nj = j + offset[1];
                        int nk = k + offset[2];

                        if (ni >= 0 && nj >= 0 && nk >= 0 && ni < N[0] && nj < N[1] && nk < N[2]) {
                            Particle *neighbour = grid[ni][nj][nk].get();
                            if (!neighbour) {
                                SPDLOG_WARN("Null neighbour pointer at grid[{}][{}][{}]", ni, nj, nk);
                                continue; // Skip null pointers
                            }

                            if (grid[ni][nj][nk]) {
                                current->addDirectNeighbour(neighbour->getID());
                            } else {
                                current->addDiagonalNeighbour(neighbour->getID());
                            }
                        }
                    }
                }
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
    SPDLOG_DEBUG("generating disc particles");
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
}
