//
// Created by Stefanie Blattenberger on 27/11/2024.
//
#pragma once

#include "LinkedCellContainer.h"
#include <list>
#include <memory>


class BoundaryHandler{

    private:
    /** the sigma value used for calculating the Lennard-Jones Potential */
    double sigma;
    /** type 0: Outflow
     *  type 1: Reflecting
     */
    const bool type; 
    /** ParticleContainer that the BoundaryHandler operates on */
    ParticleContainers::LinkedCellContainer & container;

    /**minimal distance between two particles to be repulsing each other*/
    const double minDist;

    /**boundaries of the simulation: left, right, top, bottom(, front, back) */
    const std::array<double, 6> boundaries;

    /**Deletes particles that have left the boundaries of the simulation */
    void handleOutflow();

    /**Creates shadow particles to repel particles that are about to leave the boundaries of the simulation and deletes unnecessary shadow particles */
    void handleReflecting();

    public:

    BoundaryHandler(double s, bool t, ParticleContainers::LinkedCellContainer& pc);
    ~BoundaryHandler() = default;

    /**Handles particles that have left/are about to leave the boundaries of the simulation
     * This method should be called in every iteration after updating the positions of all particles
     */
    void handleBoundaries();

    /**Calculates the absolute value of a particle to a boundary
     * @param p the Particle whose distance is to be calculated
     * @param i int that signifies the currently observed boundary: 1 -> left, 2 -> right, 3 -> top, 4 -> bottom, 5 -> front, 6 -> back
     * @returns double absolute distance of particle to boundary
     */
    double calculateDistance(Particle p, int i);

    /**Calculates the location of the ghost particle belonging to the current particle
     * @param particle
     * @param i int that signifies the currently observed boundary: 1 -> left, 2 -> right, 3 -> top, 4 -> bottom, 5 -> front, 6 -> back
     * @returns position array of ghost particle
     */

    std::array<double, 3UL> ghostParticleLocation(Particle p, int i, double dist);
};