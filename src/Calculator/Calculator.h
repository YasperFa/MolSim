//
// Created by Celik Yucel on 02.11.24.
//

#pragma once
class Particle;
#include <Objects/Particle.h>
#include "Objects/ParticleContainer.h"
#include <cmath>
#include <functional> 
#include "spdlog/spdlog.h"
#include "utils/ArrayUtils.h"


namespace Calculators {
    class Calculator {
    public:
        Calculator() = default;

        virtual ~Calculator() = default;

        virtual void calculateXFV(ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateXFV");
            calculateX(particleContainer, delta_t);
            calculateF(particleContainer);
            calculateV(particleContainer, delta_t);
        }


        /**
        * calculate the force for all particles
        */
        virtual void calculateF(ParticleContainer &particleContainer) {
            SPDLOG_TRACE("executing calculateF");
            // initialize sigma with zeros
            std::array<double, 3> sigma = {0.0,0.0,0.0};

            for (auto &p : particleContainer) {
                p.setOldF(p.getF());
                p.setF(sigma);
            }
            for (auto &pair : particleContainer.getParticlePairs()) {
                Particle &p1 = pair.first.get();
                Particle &p2 = pair.second.get();
                std::array<double, 3> sub = operator-(p2.getX(), p1.getX());
                double normCubed = pow(ArrayUtils::L2Norm(sub),3.0);
                //prevent division by 0
                if (normCubed < 1e-8) {
                    continue;
                }
                // calculate Force between the current pair of particles
                std::array<double, 3> fij = calculateFIJ(sub,p1.getM(),p2.getM(),normCubed);
                SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], p1.getID(), p2.getID());
                // add force of this pair to the overall force of particle 1
                p1.setF(operator+(p1.getF(), fij));
                // make use of Newton's third law and add the negative force calculated above to particle 2
                p2.setF(operator-(p2.getF(),fij));
            }
        }
        /**
        * calculate the force between particle i and j
        */
        virtual std::array<double, 3> calculateFIJ(const std::array<double,3> &sub, double m1, double m2, double normCubed) {
            return operator*(1.0/normCubed, operator*(m1*m2, sub));
        }

        /**
         * calculate the position for all particles
         */
        virtual  void calculateX(ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateX");
            for (auto &p : particleContainer) {
                std::array<double, 3> newX = operator+(p.getX(), operator+( operator*(delta_t, p.getV()) , operator*(0.5*pow(delta_t,2)/p.getM(), p.getF())));
                p.setX(newX);
            }
        }

        /**
         * calculate the position for all particles
         */
        virtual void calculateV(ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateV");
            for (auto &particle: particleContainer) {
                std::array<double, 3> newV = operator+(particle.getV(), operator*(delta_t*0.5/particle.getM(), operator+(particle.getOldF(),particle.getF())));
                particle.setV(newV);
            }
        }

    };
}
