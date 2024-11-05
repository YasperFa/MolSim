//
// Created by Sawsan Farah on 03/11/2024.
//

#include "Cuboid.h"
#include "../utils/MaxwellBoltzmannDistribution.h"

Cuboid::Cuboid(const std::array<double, 3> x, const std::array<double, 3> N, const double h, const double m,
    const std::array<double, 3> v,const double mv)
    : x(x), N(N), h(h), m(m), v(v), mv(mv){}


void Cuboid::generate_particles(ParticleContainer &particles) {
    // iterate over the specified dimensions and generate particles
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