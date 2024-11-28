//
// Created by Celik Yucel on 02.11.24.
//

#pragma once
class Particle;
#include <Objects/Particle.h>
#include "../Objects/Containers/DirectSum/DirectSumContainer.h"
#include <cmath>
#include <functional> 
#include "spdlog/spdlog.h"
#include "utils/ArrayUtils.h"


namespace Calculators {
    class Calculator {
    public:
        Calculator() = default;

        virtual ~Calculator() = default;

        virtual void calculateXFV(DirectSumContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateXFV");
            calculateX(particleContainer, delta_t);
            calculateF(particleContainer);
            calculateV(particleContainer, delta_t);
        }


        /**
        * calculate the force for all particles
        */
        void calculateF(DirectSumContainer &particleContainer) {
            SPDLOG_TRACE("executing calculateF");
            // initialize sigma with zeros
            std::array<double, 3> sigma = {0.0,0.0,0.0};

            for (auto &p : particleContainer) {
                p.setOldF(p.getF());
                p.setF(sigma);
            }
            for (auto it1 = particleContainer.begin(); it1 != particleContainer.end(); ++it1) {
                for (auto it2 = it1 + 1; it2 != particleContainer.end(); ++it2) {
                    std::array<double, 3> sub = operator-(it2->getX(), it1->getX());
                    double normCubed = ArrayUtils::L2Norm(sub);


                    // calculate Force between the current pair of particles
                    std::array<double, 3> fij = calculateFIJ(sub,it1->getM(),it2->getM(),normCubed);
                    SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], it1->getID(), it2->getID());
                    // add force of this pair to the overall force of particle 1
                    it1->setF(operator+(it1->getF(), fij));
                    // make use of Newton's third law and add the negative force calculated above to particle 2
                    it2->setF(operator-(it2->getF(),fij));
                }
            }

        }

        /**
        * calculate the force between particle i and j
        */
        virtual std::array<double, 3> calculateFIJ(const std::array<double,3> &sub, double m1, double m2, double normCubed) = 0;

        /**
         * calculate the position for all particles
         */
        virtual  void calculateX(DirectSumContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateX");
            for (auto &p : particleContainer) {
                std::array<double, 3> newX = operator+(p.getX(), operator+( operator*(delta_t, p.getV()) , operator*(0.5*pow(delta_t,2)/p.getM(), p.getF())));
                p.setX(newX);
            }
        }

        /**
         * calculate the position for all particles
         */
        virtual void calculateV(DirectSumContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateV");
            for (auto &particle: particleContainer) {
                std::array<double, 3> newV = operator+(particle.getV(), operator*(delta_t*0.5/particle.getM(), operator+(particle.getOldF(),particle.getF())));
                particle.setV(newV);
            }
        }
        virtual std::string toString() = 0;
    };
}
