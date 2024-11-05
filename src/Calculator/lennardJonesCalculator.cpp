//
// Created by Yasmine Farah on 05/11/2024.
//

#include "lennardJonesCalculator.h"
#include "utils/ArrayUtils.h"
#include <cmath>
#include "Objects/ParticleContainer.h"
#include "Objects/Particle.h"


static const double epsilon = 5;
static const double small_sigma = 1;
void Calculators::lennardJonesCalculator::calculateF(ParticleContainer &particleContainer) {
    // initialize sigma with zeros
    std::array<double, 3> sigma = {.0,.0,.0};
    // move the current F values to Oldf, initialize current F values with sigma
    for (auto &p : particleContainer) {
        p.setOldF(p.getF());
        p.setF(sigma);
    }
    // iterate over particle pairs
    for (auto &pair : particleContainer.getParticlePairs()) {
        Particle &p1 = pair.first.get();
        Particle &p2 = pair.second.get();
        // calculate Force between the current pair of particles
        std::array<double, 3> sub = subtractVector(p1.getX(), p2.getX());
        double normCubed = pow(magnitude(sub),3);
        double potential_dirivative = -24*epsilon*(pow((small_sigma/normCubed),6) - 2*pow((small_sigma/normCubed),12))/normCubed;
        std::array<double,3> fij = multiply_constant_vector(sub,potential_dirivative/normCubed);
        // add force of this pair to the overall force of particle 1
        p1.setF(addVector(p1.getF(),fij));
        // make use of Newton's third law and add the negative force calculated above to particle 2
        p2.setF(subtractVector(p2.getF(),fij));
    }

}




