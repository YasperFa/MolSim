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
        * @param gravity the gravitational acceleration
        */
        void calculateXFV(ParticleContainers::ParticleContainer &particleContainer, double delta_t, double gravity = 0.0) {
            SPDLOG_TRACE("executing calculateXFV");
            calculateX(particleContainer, delta_t);
            calculateF(particleContainer, gravity);
            calculateV(particleContainer, delta_t);
            if (auto lcCont = dynamic_cast<ParticleContainers::LinkedCellContainer *>(&particleContainer)) {
                lcCont->updateParticlesInCell();
            }
        }

        /**
        * calculate the force for all particles
        * @param particleContainer the container that is operated on
        */
        void calculateF(ParticleContainers::ParticleContainer &particleContainer, double gravity = 0.0) {
            SPDLOG_TRACE("executing calculateF");
            // initialize sigma with zeros
            std::array<double, 3> sigma = {0.0, 0.0, 0.0};

            for (auto &p: particleContainer) {

                if (p.getState() == Particle::State::DEAD){
                            continue;
                        }

                p.setOldF(p.getF());
                sigma[1] = p.getM() * gravity; //add gravitational force in y direction
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
                    std::array<double, 3> sub = it2->getX() - it1->getX();
                    double norm = ArrayUtils::L2Norm(sub);

                    // calculate Force between the current pair of particles
                    std::array<double, 3> fij = calculateFIJ(sub, it1->getM(), it2->getM(), norm, it1-> getEpsilon(), it2->getEpsilon(), it1 -> getSigma(), it2-> getSigma());
                    SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], it1->getID(), it2->getID());
                    // add force of this pair to the overall force of particle 1
                    it1->setF(it1->getF() + fij);
                    // make use of Newton's third law and add the negative force calculated above to particle 2
                    it2->setF(it2->getF() - fij);
                }
            }
        }

         /**Calculates the force for all particles in a LinkedCellContainer 
         * @param particleContainer the LinkedCellContainer that is operated on
         */
        void calculateFLinkedCell(ParticleContainers::LinkedCellContainer &lcCon) {
            for (auto itCell = lcCon.beginCells(); itCell != lcCon.endCells(); ++itCell) {

                if(itCell ->getCellType() == Cell::CType::HALO){ //halo cells copy behaviour of opposite boundary cells
                    continue;
                }

                for (auto itParticle1 = itCell->beginParticle(); itParticle1 != itCell->endParticle(); ++itParticle1) {
                    if (*itParticle1 == nullptr ||  (*itParticle1)->getState() == Particle::State::DEAD) {
                        continue;
                    }

                    for (auto itParticle2 = itParticle1 + 1; itParticle2 != itCell->endParticle(); ++itParticle2) {
                        if (*itParticle2 == nullptr || (*itParticle2)->getState() == Particle::State::DEAD) {
                            continue;
                        }

                        std::array<double, 3> sub = (*itParticle2)->getX() - (*itParticle1)->getX();
                        double normCubed = ArrayUtils::L2Norm(sub);

                        // calculate Force between the current pair of particles
                        std::array<double, 3> fij = calculateFIJ(sub, (*itParticle1)->getM(), (*itParticle2)->getM(),
                                                                 normCubed, (*itParticle1)->getEpsilon(), (*itParticle2)->getEpsilon(), (*itParticle1)->getSigma(), (*itParticle2)->getSigma());
                        SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], (*itParticle1)->getID(),
                                     (*itParticle2)->getID());
                        // add force of this pair to the overall force of particle 1
                        (*itParticle1)->setF((*itParticle1)->getF() + fij);
                        // make use of Newton's third law and add the negative force calculated above to particle 2
                        (*itParticle2)->setF((*itParticle2)->getF() - fij);
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

                        if ((*itParticle1)->getState() == Particle::State::DEAD){
                            continue;
                        }

                        for (Particle *neighbourP: neighbourCell->getParticlesInCell()) {
                         //   SPDLOG_INFO("PASSED 2.1");
                            if (neighbourP == nullptr || (neighbourP)->getState() == Particle::State::DEAD) {
                                continue;
                            }

                            std::array<double, 3> sub = (neighbourP->getX() -(*itParticle1)->getX());
                            double normL2 = ArrayUtils::L2Norm(sub);
                            if (normL2 > lcCon.getCutoff()) {
                                continue;
                            }


                            std::array<double, 3> fij = calculateFIJ(sub, (*itParticle1)->getM(), neighbourP->getM(),
                                                                     normL2, (*itParticle1)->getEpsilon(), neighbourP->getEpsilon(), (*itParticle1)->getSigma(), neighbourP->getSigma());
                            SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], (*itParticle1)->getID(),
                                         neighbourP->getID());
                            // add force of this pair to the overall force of particle 1
                            (*itParticle1)->setF(((*itParticle1)->getF() + fij));
                            // make use of Newton's third law and add the negative force calculated above to particle 2
                            neighbourP->setF((neighbourP->getF() - fij));
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
        * @param epsilon1 the Lennard-Jones parameter epsilon of i
        * @param epsilon2 the Lennard-Jones parameter epsilon of j
        * @param sigma1 the Lennard-Jones parameter sigma of i
        * @param sigma2 the Lennard-Jones parameter sigma of j
        * @return force between i and j
        */
        virtual std::array<double, 3> calculateFIJ(const std::array<double, 3> &sub, double m1, double m2,
                                                   double normCubed, double epsilon1, double epsilon2, double sigma1, double sigma2) = 0;

        /**
         * calculate the position for all particles
         * @param particleContainer the container that is operated on
         * @param delta_t timestep between iterations
         */
        void calculateX(ParticleContainers::ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateX");
            for (auto &p: particleContainer) {
                if (p.getState() == Particle::State::DEAD){
                            continue;
                        }

                p.setOldX(p.getX());
                std::array<double, 3> newX = (
                    p.getX() + ((delta_t * p.getV()) +
                                        (0.5 * pow(delta_t, 2) / p.getM()* p.getF())));
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

                if (particle.getState() == Particle::State::DEAD){
                            continue;
                        }
                std::array<double, 3> newV = (particle.getV() +
                                                       (delta_t * 0.5 / particle.getM() *
                                                                 (particle.getOldF() + particle.getF())));
                particle.setV(newV);
            }
        }
        virtual std::string toString() = 0;
    };
}
