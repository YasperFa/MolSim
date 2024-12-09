//
// Created by Sawsan Farah on 10/11/2024.
//

#include "ParticleGenerator.h"

#include "Containers/ParticleContainer.h"
#include "spdlog/spdlog.h"


void ParticleGenerator::generateCuboid(ParticleContainers::ParticleContainer &particles, Cuboid &cuboid, int type, double epsilon, double sigma) {
    // iterate over the specified dimensions and generate particles
    SPDLOG_DEBUG("generating cuboid particles");
    std::array<double,3> N = cuboid.getNumOfParticlesPerDimension();
    std::array<double,3> x = cuboid.getLowerFrontCorner();
    std::array<double,3> v = cuboid.getInitVelocity();
    double h = cuboid.getDistBetweenParticles();
    double m = cuboid.getMass();
    for (int k=0; k < N[2];++k) {
        for (int j=0; j < N[1];++j) {
            for (int i=0; i < N[0];++i) {
                // calculate x,y,z coordinates
                double x_coor = x[0] + i*h;
                double y_coor = x[1] + j*h;
                double z_coor = x[2] + k*h;
                std::array<double, 3> particle_pos = {x_coor,y_coor,z_coor};
                // create new particle
                Particle nParticle(particle_pos,v,m, type, epsilon, sigma);
                // add new particle to container
                particles.addParticle(nParticle);
            }
        }
    }
}

void ParticleGenerator::generateDisc(ParticleContainers::ParticleContainer &particles, Disc &disc, int type, double epsilon, double sigma) {
    const std::array<double, 3> center = disc.getCenterCoordinate();
    const  std::array<double, 3> initVel = disc.getInitVelocity();
    const int r = disc.getRadius();
    const double mass = disc.getMass();
    const double h = disc.getDistanceBetweenParticles();
    SPDLOG_DEBUG("generating disc particles");
    SPDLOG_DEBUG("r: {}", r);
    for (int i = -r ; i <= r ; ++i) {
        SPDLOG_DEBUG("column {}", i);
        // check if position is inside the circle j*j + i*i <= r*r
        for (int j = -r ; j <= r; ++j) {
            if (((j*j) + (i*i) <= r*r)) {
                SPDLOG_DEBUG("line {}", j);
                const std::array<double, 3> particlePosition = {center[0] + j*h, center[1] + i*h, center[2]};
                // create new particle
                Particle nParticle(particlePosition,initVel, mass, type, epsilon, sigma);
                // add new particle to container
                particles.addParticle(nParticle);
            }
        }
    }


}
