//
// Created by Sawsan Farah on 10/11/2024.
//

#include "ParticleGenerator.h"

#include "spdlog/spdlog.h"
#include "utils/MaxwellBoltzmannDistribution.h"


void ParticleGenerator::generateCuboid(ParticleContainer &particles, Cuboid &cuboid) {
    // iterate over the specified dimensions and generate particles
    SPDLOG_DEBUG("generating cuboid particles");
    std::array<double,3> N = cuboid.getNumOfParticlesPerDimension();
    std::array<double,3> x = cuboid.getLowerFrontCorner();
    std::array<double,3> v = cuboid.getInitVelocity();
    double h = cuboid.getDistBetweenParticles();
    double mv = cuboid.getMeanVelocity();
    double m = cuboid.getMass();
    for (int k=0; k < N[2];++k) {
        for (int j=0; j < N[1];++j) {
            for (int i=0; i < N[0];++i) {
                // calculate x,y,z coordinates
                double x_coor = x[0] + i*h;
                double y_coor = x[1] + j*h;
                double z_coor = x[2] + k*h;
                std::array<double, 3> particle_pos = {x_coor,y_coor,z_coor};
                // get the maxwell velocity
                std::array<double, 3> maxwell_vel = maxwellBoltzmannDistributedVelocity(mv,2);
                // get the initial velocity
                std::array<double, 3> vel = v;
                // add maxwell velocity to initial velocity? --> I don't know if this is what we are supposed
                for (int m = 0; m < 3; ++m) {
                    vel[m] += maxwell_vel[m];
                }
                // create new particle
                Particle nParticle(particle_pos,vel,m,0);
                // add new particle to container
                particles.addParticle(nParticle);


            }
        }
    }
}

void ParticleGenerator::generateDisc(ParticleContainer &particles, Disc &disc) {
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
                Particle nParticle(particlePosition,initVel, mass,0);
                // add new particle to container
                particles.addParticle(nParticle);
            }
        }
    }


}
