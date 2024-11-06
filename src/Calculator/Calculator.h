//
// Created by Celik Yucel on 02.11.24.
//

#pragma once
class Particle;
#include <Objects/Particle.h>
#include "Objects/ParticleContainer.h"
#include <cmath>
#include <functional> 


namespace Calculators {
    class Calculator {
    public:
        Calculator() = default;

        virtual ~Calculator() = default;

        virtual void calculateXFV(ParticleContainer &particleContainer, double delta_t) {
            calculateX(particleContainer, delta_t);
            calculateF(particleContainer);
            calculateV(particleContainer, delta_t);
        }


        /**
        * calculate the force for all particles
        */
        virtual void calculateF(ParticleContainer &particleContainer) {
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
                if (normCubed == 0) {
                    continue;
                }


                std::array<double, 3> fij = multiply_constant_vector(multiply_constant_vector(sub, p1.getM()*p2.getM()),1/normCubed);

                p1.setF(addVector(p1.getF(),fij));
                p2.setF(subtractVector(p2.getF(),fij));
            }
        }


        /**
         * calculate the position for all particles
         */
        virtual  void calculateX(ParticleContainer &particleContainer, double delta_t) {
            for (auto &p : particleContainer) {
                std::array<double, 3> newX = addVector(p.getX(), addVector( multiply_constant_vector(p.getV(),delta_t) , multiply_constant_vector(p.getOldF(),0.5*pow(delta_t,2)/p.getM())));
                p.setX(newX);
            }
        }

        /**
         * calculate the position for all particles
         */
        virtual void calculateV(ParticleContainer &particleContainer, double delta_t) {
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
            double out = 0;
            for (int i = 0; i < 3; ++i) {
                out += pow(a[i], 2);
            }
            return std::sqrt(out);
        }
    };

}
