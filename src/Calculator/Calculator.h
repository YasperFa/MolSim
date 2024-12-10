//
// Created by Celik Yucel on 02.11.24.
//

#pragma once
#include "Objects/Containers/LinkedCell/LinkedCellContainer.h"
class Particle;
#include <Objects/Particle.h>
#include "../Objects/Containers/DirectSum/DirectSumContainer.h"
#include <cmath>
#include <functional>
#include "spdlog/spdlog.h"
#include "utils/ArrayUtils.h"
#include "../Objects/Containers/LinkedCell/BoundaryHandler.h"


namespace Calculators {
    class Calculator {
    public:
        Calculator() = default;

        virtual ~Calculator() = default;

        /**
        * Updates all particle values for one iteration and updates the container if necessary
        * @param particleContainer the container that is operated on
        * @param delta_t timestep between iterations
        */
        void calculateXFV(ParticleContainers::ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateXFV");
            calculateX(particleContainer, delta_t);
            calculateF(particleContainer);
            calculateV(particleContainer, delta_t);
            if (auto lcCont = dynamic_cast<ParticleContainers::LinkedCellContainer *>(&particleContainer)) {
                lcCont->updateParticlesInCell();
            }
        }

        /**
        * calculate the force for all particles
        * @param particleContainer the container that is operated on
        */
        void calculateF(ParticleContainers::ParticleContainer &particleContainer) {
            SPDLOG_TRACE("executing calculateF");
            // initialize sigma with zeros
            std::array<double, 3> sigma = {0.0, 0.0, 0.0};

            for (auto &p: particleContainer) {
                p.setOldF(p.getF());
                p.setF(sigma);
            }

            if (auto dsCont = dynamic_cast<ParticleContainers::DirectSumContainer *>(&particleContainer)) {
                calculateFDirectSum(*dsCont);
            } else if (auto lcCont = dynamic_cast<ParticleContainers::LinkedCellContainer *>(&particleContainer)) {
                calculateFLinkedCell(*lcCont);
            }
        }

        /**Calculates the force for all particles in a DirectSumContainer 
         * @param particleContainer the DirectSumContainer that is operated on
        */
        
        void calculateFDirectSum(ParticleContainers::DirectSumContainer &particleContainer) {
            for (auto it1 = particleContainer.begin(); it1 != particleContainer.end(); ++it1) {
                for (auto it2 = it1 + 1; it2 != particleContainer.end(); ++it2) {
                    std::array<double, 3> sub = operator-(it2->getX(), it1->getX());
                    double norm = ArrayUtils::L2Norm(sub);

                    // calculate Force between the current pair of particles
                    std::array<double, 3> fij = calculateFIJ(sub, it1->getM(), it2->getM(), norm);
                    SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], it1->getID(), it2->getID());
                    // add force of this pair to the overall force of particle 1
                    it1->setF(operator+(it1->getF(), fij));
                    // make use of Newton's third law and add the negative force calculated above to particle 2
                    it2->setF(operator-(it2->getF(), fij));
                }
            }
        }

         /**Calculates the force for all particles in a LinkedCellContainer 
         * @param particleContainer the LinkedCellContainer that is operated on
         */
        void calculateFLinkedCell(ParticleContainers::LinkedCellContainer &lcCon) {
            for (auto itCell = lcCon.beginCells(); itCell != lcCon.endCells(); ++itCell) {
                for (auto itParticle1 = itCell->beginParticle(); itParticle1 != itCell->endParticle(); ++itParticle1) {
                    if (*itParticle1 == nullptr) {
                        continue;
                    }
                    for (auto itParticle2 = itParticle1 + 1; itParticle2 != itCell->endParticle(); ++itParticle2) {
                        if (*itParticle2 == nullptr) {
                            continue;
                        }

                        std::array<double, 3> sub = operator-((*itParticle2)->getX(), (*itParticle1)->getX());
                        double normCubed = ArrayUtils::L2Norm(sub);

                        // calculate Force between the current pair of particles
                        std::array<double, 3> fij = calculateFIJ(sub, (*itParticle1)->getM(), (*itParticle2)->getM(),
                                                                 normCubed);
                        SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], (*itParticle1)->getID(),
                                     (*itParticle2)->getID());
                        // add force of this pair to the overall force of particle 1
                        (*itParticle1)->setF(operator+((*itParticle1)->getF(), fij));
                        // make use of Newton's third law and add the negative force calculated above to particle 2
                        (*itParticle2)->setF(operator-((*itParticle2)->getF(), fij));
                    }

                }
                for (Cell *neighbourCell: itCell->getNeighbourCells()) {
                    if (neighbourCell == nullptr) {
                        continue;
                    }

                    if (itCell->getInfluencedByCells().find(neighbourCell) != itCell->endInfluencedBy()) {
                        continue;
                    }
                    for (auto itParticle1 = itCell->beginParticle(); itParticle1 != itCell->endParticle(); ++itParticle1) {
                        for (Particle *neighbourP: neighbourCell->getParticlesInCell()) {
                         //   SPDLOG_INFO("PASSED 2.1");
                            if (neighbourP == nullptr) {
                                continue;
                            }

                            std::array<double, 3> sub = operator-(neighbourP->getX(),(*itParticle1)->getX());
                            double normL2 = ArrayUtils::L2Norm(sub);
                            if (normL2 > lcCon.getCutoff()) {
                                continue;
                            }


                            std::array<double, 3> fij = calculateFIJ(sub, (*itParticle1)->getM(), neighbourP->getM(),
                                                                     normL2);
                            SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], (*itParticle1)->getID(),
                                         neighbourP->getID());
                            // add force of this pair to the overall force of particle 1
                            (*itParticle1)->setF(operator+((*itParticle1)->getF(), fij));
                            // make use of Newton's third law and add the negative force calculated above to particle 2
                            neighbourP->setF(operator-(neighbourP->getF(), fij));
                        }
                        neighbourCell->addInfluencedByCell(&(*itCell));
                       // SPDLOG_INFO("PASSED 2.2");
                    }
                }
            }
        }

        /**
        * calculate the force between particle i and j 
        * @param sub difference of the positions of j and i
        * @param m1 mass of i
        * @param m2 mass of j
        * @param normCubed norm of sub
        * @return force between i and j
        */
        virtual std::array<double, 3> calculateFIJ(const std::array<double, 3> &sub, double m1, double m2,
                                                   double normCubed) = 0;

        /**
         * calculate the position for all particles
         * @param particleContainer the container that is operated on
         * @param delta_t timestep between iterations
         */
        void calculateX(ParticleContainers::ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateX");
            for (auto &p: particleContainer) {
                std::array<double, 3> newX = operator+(
                    p.getX(), operator+(operator*(delta_t, p.getV()),
                                        operator*(0.5 * pow(delta_t, 2) / p.getM(), p.getF())));
                p.setX(newX);
            }
        }

        /**
         * calculate the position for all particles
         * @param particleContainer the container that is operated on
         * @param delta_t timestep between iterations
         */
        void calculateV(ParticleContainers::ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateV");
            for (auto &particle: particleContainer) {
                std::array<double, 3> newV = operator+(particle.getV(),
                                                       operator*(delta_t * 0.5 / particle.getM(),
                                                                 operator+(particle.getOldF(), particle.getF())));
                particle.setV(newV);
            }
        }
        virtual std::string toString() = 0;
    };
}
