//
// Created by Sawsan Farah on 10/11/2024.
//

#include "ParticleGenerator.h"

#include "spdlog/spdlog.h"
#include "utils/MaxwellBoltzmannDistribution.h"


void ParticleGenerator::generateCuboid(ParticleContainer &particles, Cuboid &cuboid) {
    // iterate over the specified dimensions and generate particles
    SPDLOG_DEBUG("generating cuboid particles");
    std::array<double,3> N = cuboid.get_num_of_particlespDimension();
    std::array<double,3> x = cuboid.get_lower_front_corner();
    std::array<double,3> v = cuboid.get_init_velocity();
    double h = cuboid.get_dist_pasrticles();
    double mv = cuboid.get_mean_velocity();
    double m = cuboid.get_mass();
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