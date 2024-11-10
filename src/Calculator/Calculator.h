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
            std::array<double, 3> sigma = {.0,.0,.0};

            for (auto &p : particleContainer) {
                p.setOldF(p.getF());
                p.setF(sigma);
            }
            for (auto &pair : particleContainer.getParticlePairs()) {
                Particle &p1 = pair.first.get();
                Particle &p2 = pair.second.get();
                std::array<double, 3> sub = subtractVector(p2.getX(), p1.getX());
                double normCubed = pow(magnitude(sub),3);
                //prevent division by 0
                if (normCubed < 1e-8) {
                    continue;
                }
                // calculate Force between the current pair of particles
                std::array<double, 3> fij = calculateFIJ(sub,p1.getM(),p2.getM(),normCubed);
                // add force of this pair to the overall force of particle 1
                p1.setF(addVector(p1.getF(),fij));
                // make use of Newton's third law and add the negative force calculated above to particle 2
                p2.setF(subtractVector(p2.getF(),fij));
            }
        }
        /**
        * calculate the force between particle i and j
        */
        virtual std::array<double, 3> calculateFIJ(const std::array<double,3> &sub, double m1, double m2, double normCubed) {
            return multiply_constant_vector(multiply_constant_vector(sub, m1*m2),1/normCubed);
        }

        /**
         * calculate the position for all particles
         */
        virtual  void calculateX(ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateX");
            for (auto &p : particleContainer) {
                std::array<double, 3> newX = addVector(p.getX(), addVector( multiply_constant_vector(p.getV(),delta_t) , multiply_constant_vector(p.getF(),0.5*pow(delta_t,2)/p.getM())));
                p.setX(newX);
            }
        }

        /**
         * calculate the position for all particles
         */
        virtual void calculateV(ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateV");
            for (auto &particle: particleContainer) {
                std::array<double, 3> newV = addVector(particle.getV(), multiply_constant_vector(addVector(particle.getOldF(),particle.getF()),delta_t*0.5/particle.getM()));
                particle.setV(newV);
            }
        }

        /**
        * helper function for adding two vectors
        * @param a first vector
        * @param b second vector
        * @returns vector sum of a and b
        */
        virtual  std::array<double, 3> addVector(const std::array<double, 3> &a, const std::array<double, 3> &b) {
            //SPDLOG_TRACE("adding vectors");
            std::array<double, 3> out{};
            for (int i = 0; i < 3; ++i) {
                out[i] = a[i] + b[i];
            }
            return out;
        }


        /**
         * helper function for subtracting two vectors
         * @param a first vector
         * @param b second vector
         * @returns vector difference of a and b
         */
        virtual  std::array<double, 3> subtractVector(const std::array<double, 3> &a, const std::array<double, 3> &b) {
            //SPDLOG_TRACE("subtracting vectors");
            std::array<double, 3> out{};
            for (int i = 0; i < 3; ++i) {
                out[i] = a[i] - b[i];
            }
            return out;
        }

        /**
         * helper function for multiplying a vector with constant
         * @param a first vector
         * @param b double value
         * @returns product of a and b
         */
        virtual  std::array<double, 3> multiply_constant_vector(const std::array<double, 3> &a, const double b) {
            //SPDLOG_TRACE("multiplying vector with constant");
            std::array<double, 3> out{};
            for (int i = 0; i < 3; ++i) {
                out[i] = a[i] * b;
            }
            return out;
        }

        /**
         * helper function for calculating magnitude of a vector
         * @param a vector
         * @returns magnitude of a (double)
         */
        virtual double magnitude(const std::array<double, 3> &a) {
            //SPDLOG_TRACE("calculating magnitude");
            double out = 0;
            for (int i = 0; i < 3; ++i) {
                out += pow(a[i], 2);
            }
            return std::sqrt(out);
        }
    };

}
