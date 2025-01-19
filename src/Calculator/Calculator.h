//
// Created by Celik Yucel on 02.11.24.
//

#pragma once
#include "HarmonicForceCalculator.h"
#include "Objects/Containers/LinkedCell/LinkedCellContainer.h"
class Particle;
#include <Objects/Particle.h>
#include "../Objects/Containers/DirectSum/DirectSumContainer.h"
#include <cmath>
#include <functional>
#include "spdlog/spdlog.h"
#include "utils/ArrayUtils.h"
#include "../Objects/Containers/LinkedCell/BoundaryHandler.h"
#include <boost/functional/hash.hpp>


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
        void calculateXFV(ParticleContainers::ParticleContainer &particleContainer, double delta_t,
                          double gravity = 0.0, bool harmonicOn = false,
                          double stiffnessConstant = 0.0, double avgBondLength = 0.0, double upwardsForce = 0.0,
                          int activeTimesteps = 0, int gravityAxis = 1, int specialForceAxis = 1) {
            SPDLOG_TRACE("executing calculateXFV");
            calculateX(particleContainer, delta_t);
            calculateF(particleContainer, gravity, harmonicOn, stiffnessConstant, avgBondLength, upwardsForce,
                       activeTimesteps, gravityAxis, specialForceAxis);
            calculateV(particleContainer, delta_t);
            if (auto lcCont = dynamic_cast<ParticleContainers::LinkedCellContainer *>(&particleContainer)) {
                lcCont->updateParticlesInCell();
            }
        }

        /**
        * calculate the force for all particles
        * @param particleContainer the container that is operated on
        * @param gravity
        * @param stiffnessConstant
        * @param avgBondLength
        */
        void calculateF(ParticleContainers::ParticleContainer &particleContainer, double gravity = 0.0,
                        bool harmonicOn = false,
                        double stiffnessConstant = 0.0, double avgBondLength = 0.0, double upwardsForce = 0.0,
                        int activeTimesteps = 0, int gravityAxis = 1, int specialForceAxis = 1) {
            SPDLOG_TRACE("executing calculateF");
            // initialize sigma with zeros
            std::array<double, 3> sigma = {0.0, 0.0, 0.0};

            for (auto &p: particleContainer) {
                p.setOldF(p.getF());
                sigma[gravityAxis] = p.getM() * gravity; //add gravitaional force in y direction

                if (activeTimesteps > 0 && p.upwardForceMarked()) {
                    sigma[specialForceAxis] += upwardsForce;
                }

                p.setF(sigma);
            }

            if (auto dsCont = dynamic_cast<ParticleContainers::DirectSumContainer *>(&particleContainer)) {
                calculateFDirectSum(*dsCont);
            } else if (auto lcCont = dynamic_cast<ParticleContainers::LinkedCellContainer *>(&particleContainer)) {
                if (lcCont->version2) {
                    calculateFLinkedCellV2(*lcCont);
                } else {
                    calculateFLinkedCell(*lcCont);
                }
            }

            if (harmonicOn) {
                HarmonicForceCalculator harmonicForceCalculator(stiffnessConstant, avgBondLength);
                applyHarmonicForces(particleContainer, harmonicForceCalculator);
            }
        }

        /**Calculates the force for all particles in a DirectSumContainer
         * @param particleContainer the DirectSumContainer that is operated on
        */

        void calculateFDirectSum(ParticleContainers::DirectSumContainer &particleContainer) {
#ifdef _OPENMP
        #pragma omp parallel for schedule(dynamic)
#endif
            for (auto it1 = particleContainer.begin(); it1 != particleContainer.end(); ++it1) {
                for (auto it2 = it1 + 1; it2 != particleContainer.end(); ++it2) {
                    std::array<double, 3> sub = it2->getX() - it1->getX();
                    double norm = ArrayUtils::L2Norm(sub);

                    bool neighbour = it1->isNeighbour(*it2);
                    // calculate Force between the current pair of particles
                    std::array<double, 3> fij = calculateFIJ(sub, it1->getM(), it2->getM(), norm, it1->getEpsilon(),
                                                             it2->getEpsilon(), it1->getSigma(), it2->getSigma(),
                                                             neighbour);
                    SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], it1->getID(), it2->getID());
                    // add force of this pair to the overall force of particle 1
                    if (!it1->getFixed())
                        it1->setF(it1->getF() + fij);
                    // make use of Newton's third law and add the negative force calculated above to particle 2
                    if (!it2->getFixed())
                        it2->setF(it2->getF() - fij);
                }
            }
        }

        /**Calculates the force for all particles in a LinkedCellContainer
        * @param lcCon the LinkedCellContainer that is operated on
        */
        void calculateFLinkedCell(ParticleContainers::LinkedCellContainer &lcCon) {
#ifdef _OPENMP
        #pragma omp parallel for schedule(dynamic)
#endif
            for (auto itCell = lcCon.beginCells(); itCell != lcCon.endCells(); ++itCell) {
                if (itCell->getCellType() == Cell::CType::HALO) {
                    //halo cells copy behaviour of opposite boundary cells
                    continue;
                }

                for (auto itParticle1 = itCell->beginParticle(); itParticle1 != itCell->endParticle(); ++itParticle1) {
                    if (*itParticle1 == nullptr) {
                        continue;
                    }
                    for (auto itParticle2 = itParticle1 + 1; itParticle2 != itCell->endParticle(); ++itParticle2) {
                        if (*itParticle2 == nullptr) {
                            continue;
                        }

                        std::array<double, 3> sub = (*itParticle2)->getX() - (*itParticle1)->getX();
                        double normCubed = ArrayUtils::L2Norm(sub);

                        bool neighbour = (*itParticle1)->isNeighbour(**itParticle2);


                        // calculate Force between the current pair of particles
                        std::array<double, 3> fij = calculateFIJ(sub, (*itParticle1)->getM(), (*itParticle2)->getM(),
                                                                 normCubed, (*itParticle1)->getEpsilon(),
                                                                 (*itParticle2)->getEpsilon(),
                                                                 (*itParticle1)->getSigma(),
                                                                 (*itParticle2)->getSigma(), neighbour);
                        SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], (*itParticle1)->getID(),
                                     (*itParticle2)->getID());
                        // add force of this pair to the overall force of particle 1
                        if (!(*itParticle1)->getFixed())
                            (*itParticle1)->setF((*itParticle1)->getF() + fij);
                        // make use of Newton's third law and add the negative force calculated above to particle 2
                        if (!(*itParticle2)->getFixed())
                            (*itParticle2)->setF((*itParticle2)->getF() - fij);
                    }
                }
                for (Cell *neighbourCell: itCell->getNeighbourCells()) {
                    if (neighbourCell == nullptr) {
                        continue;
                    }

                    if (neighbourCell < &(*itCell)) {
                        continue;
                    }
                    for (auto itParticle1 = itCell->beginParticle(); itParticle1 != itCell->endParticle(); ++
                         itParticle1) {
                        for (Particle *neighbourP: neighbourCell->getParticlesInCell()) {
                            if (neighbourP == nullptr) {
                                continue;
                            }

                            std::array<double, 3> sub = (neighbourP->getX() - (*itParticle1)->getX());
                            double normL2 = ArrayUtils::L2Norm(sub);
                            if (normL2 > lcCon.getCutoff()) {
                                continue;
                            }

                            bool neighbourParticles = (*itParticle1)->isNeighbour(*neighbourP);
                            std::array<double, 3> fij = calculateFIJ(sub, (*itParticle1)->getM(), neighbourP->getM(),
                                                                     normL2, (*itParticle1)->getEpsilon(),
                                                                     neighbourP->getEpsilon(),
                                                                     (*itParticle1)->getSigma(),
                                                                     neighbourP->getSigma(), neighbourParticles);
                            SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], (*itParticle1)->getID(),
                                         neighbourP->getID());
                            // add force of this pair to the overall force of particle 1
                            if (!(*itParticle1)->getFixed())
                                (*itParticle1)->setF(((*itParticle1)->getF() + fij));
                            // make use of Newton's third law and add the negative force calculated above to particle 2
                            if (!neighbourP->getFixed())
                                neighbourP->setF((neighbourP->getF() - fij));
                        }
                    }
                }
            }
        }

        /**Calculates the force for all particles in a LinkedCellContainer accodring to version 2 of parallelization
        * @param lcCon the LinkedCellContainer that is operated on
        */
        void calculateFLinkedCellV2(ParticleContainers::LinkedCellContainer &lcCon) {
            for (auto order: lcCon.getIterationOrders()) {
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
                for (auto itCell: order) {
                    if (itCell->getCellType() == Cell::CType::HALO) {
                        //halo cells copy behaviour of opposite boundary cells
                        continue;
                    }

                    for (auto itParticle1 = itCell->beginParticle(); itParticle1 != itCell->endParticle(); ++
                         itParticle1) {
                        if (*itParticle1 == nullptr) {
                            continue;
                        }
                        for (auto itParticle2 = itParticle1 + 1; itParticle2 != itCell->endParticle(); ++itParticle2) {
                            if (*itParticle2 == nullptr) {
                                continue;
                            }

                            std::array<double, 3> sub = (*itParticle2)->getX() - (*itParticle1)->getX();
                            double normCubed = ArrayUtils::L2Norm(sub);

                            bool neighbour = (*itParticle1)->isNeighbour(**itParticle2);

                            // calculate Force between the current pair of particles
                            std::array<double, 3> fij = calculateFIJ(sub, (*itParticle1)->getM(),
                                                                     (*itParticle2)->getM(),
                                                                     normCubed, (*itParticle1)->getEpsilon(),
                                                                     (*itParticle2)->getEpsilon(),
                                                                     (*itParticle1)->getSigma(),
                                                                     (*itParticle2)->getSigma(),
                                                                     neighbour);
                            SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], (*itParticle1)->getID(),
                                         (*itParticle2)->getID());
                            // add force of this pair to the overall force of particle 1
                            if (!(*itParticle1)->getFixed())
                                (*itParticle1)->setF((*itParticle1)->getF() + fij);
                            // make use of Newton's third law and add the negative force calculated above to particle 2
                            if (!(*itParticle2)->getFixed())
                                (*itParticle2)->setF((*itParticle2)->getF() - fij);
                        }
                    }
                    for (Cell *neighbourCell: itCell->getNeighbourCells()) {
                        if (neighbourCell == nullptr) {
                            continue;
                        }

                        if (neighbourCell < &(*itCell)) {
                            continue;
                        }
                        for (auto itParticle1 = itCell->beginParticle(); itParticle1 != itCell->endParticle(); ++
                             itParticle1) {
                            for (Particle *neighbourP: neighbourCell->getParticlesInCell()) {
                                if (neighbourP == nullptr) {
                                    continue;
                                }

                                std::array<double, 3> sub = (neighbourP->getX() - (*itParticle1)->getX());
                                double normL2 = ArrayUtils::L2Norm(sub);
                                if (normL2 > lcCon.getCutoff()) {
                                    continue;
                                }

                                bool neighbourParticles = (*itParticle1)->isNeighbour(*neighbourP);


                                std::array<double, 3> fij = calculateFIJ(
                                    sub, (*itParticle1)->getM(), neighbourP->getM(),
                                    normL2, (*itParticle1)->getEpsilon(), neighbourP->getEpsilon(),
                                    (*itParticle1)->getSigma(), neighbourP->getSigma(), neighbourParticles);
                                SPDLOG_TRACE("fij {} from particles {} and {}", fij[0], (*itParticle1)->getID(),
                                             neighbourP->getID());
                                // add force of this pair to the overall force of particle 1
                                if (!(*itParticle1)->getFixed())
                                    (*itParticle1)->setF(((*itParticle1)->getF() + fij));
                                // make use of Newton's third law and add the negative force calculated above to particle 2
                                if (!neighbourP->getFixed())
                                    neighbourP->setF((neighbourP->getF() - fij));
                            }
                        }
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
        * @param neighbours true if the two particles are neighbours
        * @return force between i and j
        */
        virtual std::array<double, 3> calculateFIJ(const std::array<double, 3> &sub, double m1, double m2,
                                                   double normCubed, double epsilon1, double epsilon2, double sigma1,
                                                   double sigma2, bool neighbours) = 0;

        /**
         * calculate the position for all particles
         * @param particleContainer the container that is operated on
         * @param delta_t timestep between iterations
         */
        void calculateX(ParticleContainers::ParticleContainer &particleContainer, double delta_t) {
            SPDLOG_TRACE("executing calculateX");
            for (auto &p: particleContainer) {
                if (p.getFixed()) continue;
                p.setOldX(p.getX());
                std::array<double, 3> newX = (
                    p.getX() + ((delta_t * p.getV()) +
                                (0.5 * pow(delta_t, 2) / p.getM() * p.getF())));
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
                if (particle.getFixed()) continue;
                std::array<double, 3> newV = (particle.getV() +
                                              (delta_t * 0.5 / particle.getM() *
                                               (particle.getOldF() + particle.getF())));
                particle.setV(newV);
            }
        }

        virtual std::string toString() = 0;

        void applyHarmonicForces(ParticleContainers::ParticleContainer &particleContainer,
                                 HarmonicForceCalculator &harmonicForceCalculator) {
            SPDLOG_TRACE("Applying harmonic forces");

            // To keep track of processed pairs
            std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> processedPairs;

            auto processNeighbours = [&](Particle &particle,
                                         const std::vector<int> &neighbourIds,
                                         ParticleContainers::ParticleContainer &container,
                                         double bondLengthMultiplier) {
                for (int neighbourId : neighbourIds) {
                    // Ensure we only process each pair once
                    int particleId = particle.getID();
                    auto pair = std::minmax(particleId, neighbourId);

                    if (processedPairs.count(pair) > 0) {
                        continue; // Skip already processed pair
                    }
                    processedPairs.insert(pair);

                    // Find the neighbour particle
                    auto it = std::find_if(container.begin(), container.end(), [&neighbourId](const Particle &p) {
                        return p.getID() == neighbourId;
                    });

                    if (it == container.end()) {
                        SPDLOG_ERROR("Neighbour with ID {} not found", neighbourId);
                        continue;
                    }

                    Particle &neighbour = *it;

                    // Calculate and apply forces
                    std::array<double, 3> sub = neighbour.getX() - particle.getX();
                    std::array<double, 3> force = harmonicForceCalculator.calculateFIJ(sub, particle, neighbour, bondLengthMultiplier);

                    if (!particle.getFixed()) {
                        particle.setF(particle.getF() + force);
                    }
                    if (!neighbour.getFixed()) {
                        neighbour.setF(neighbour.getF() - force);
                    }
                }
            };


            for (auto &particle: particleContainer) {
                processNeighbours(particle, particle.getDirectNeighbourIds(), particleContainer, 1.0);

                processNeighbours(particle, particle.getDiagonalNeighbourIds(), particleContainer, sqrt(2.0));
            }
        }
    };
}
