//
// Created by Stefanie Blattenberger on 27/11/2024.
//
#pragma once

#include "LinkedCellContainer.h"
#include <memory>

/**Class that handles particles at the boundaries of a LinkedCellContainer */
class BoundaryHandler{

     public:

    /**Enum class for the different types of boundary conditions */
    enum class bCondition{
        OUTFLOW,
        REFLECTING,
        PERIODIC,
    };


    BoundaryHandler(std::array<bCondition, 6> t, ParticleContainers::LinkedCellContainer& pc);
    ~BoundaryHandler() = default;

    /** Handles particles that have left/are about to leave the boundaries of the simulation
     * This method should be called in every iteration after updating the positions of all particles
     */
    void handleBoundaries();

    /** Calculates the absolute value of a particle to a boundary
     * @param p the Particle whose distance is to be calculated
     * @param i int that signifies the currently observed boundary: 0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 4 -> front, 5 -> back
     * @returns double absolute distance of particle to boundary
     */
    double calculateDistance(Particle p, int i);

    /** Calculates the location of the ghost particle belonging to the current particle
     * @param particle
     * @param i int that signifies the currently observed boundary: 0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 4 -> front, 5 -> back
     * @returns position array of ghost particle
     */

    std::array<double, 3UL> ghostParticleLocation(Particle p, int i, double dist);
   
   /** Checks whether a cell a halo cell of boundary i 
    * Assumes that the cell is a halo cell
    * @param i int that signifies the currently observed boundary: 0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 4 -> front, 5 -> back
    * @param pos position of the cell in the cell grid
    * @returns true if the cell at pos is a halo cell of boundary i
    */
    bool isHaloCellofBoundary(int i, std::array<int, 3> pos);

    /** Checks whether a cell a boundary cell of boundary i 
    * Assumes that the cell is a boundary cell
    * @param i int that signifies the currently observed boundary: 0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 4 -> front, 5 -> back
    * @param pos position of the cell in the cell grid
    * @returns true if the cell at pos is a boundary cell of boundary i
    */
    bool isBoundaryCellofBoundary(int i, std::array<int, 3> pos);

    /** Checks whether a particle was not in a boundary cell and not in a halo cell of boundary i in the last iteration,
     * meaning that it moved into the boundary from an inner cell
     * Assumes that particle is within boundary cell 
     * @param i int that signifies the currently observed boundary: 0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 4 -> front, 5 -> back
     * @param pos former position of the particle
     * @returns true if the particle was not in a boundary cell and not in a halo cell of boundary i in the last iteration
     * */
    bool movedIntoBoundary(int i, std::array<double, 3> oldX);

    /** Creates a clone particle for the peridic boundary handling
     * @param i int that signifies the currently observed boundary: 0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 4 -> front, 5 -> back
     * @param  particle the Particle that is to be cloned
     * @returns particle which has been cloned to the opposite side of the domain
     */
    Particle createCloneParticle(int i, Particle particle);

    /** Creates a clone particle of a corner particle for the peridic boundary handling
     * @param i int that signifies the first boundary: 0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 4 -> front, 5 -> back
     * @param j int that signifies the second boundary. i < j
     * @param  particle the Particle that is to be cloned
     * @returns particle which has been cloned to the opposite corner of the domain
     */
    Particle createCloneParticle(int i, int j, Particle particle);

    /** Function that deals with the special case of particles in corner cells for the periodic boundary handling
     * @param i int that signifies the currently observed boundary: 0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 4 -> front, 5 -> back
     * @param c current cell
     */
    void handleCornersPeriodic(int i, Cell& c);

    /** Helper function to calculate the position of a cloned particle
     * @param pos position of the particle to be cloned
     * @param i int that signifies the boundary we are cloning at: 0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 4 -> front, 5 -> back
     * @returns position of cloned particle
     */
    std::array<double, 3UL> cloneParticleLocation(std::array <double, 3UL> pos, int i);

    /** Calculates the minimal distance for two particles with LJ-Parameter sigma to repel each other
     * @param sigma the sigma parameter of the particles (both have the same sigma parameter)
     * @returns minimal distance for the particles to repel each other
     */
    double minDist(double sigma);

    private:

    /** determines what condition is used on what border
     * boundaries of the simulation: left, right, top, bottom(, front, back)
     */
    const std::array<bCondition, 6> type; 

    /** ParticleContainer that the BoundaryHandler operates on */
    ParticleContainers::LinkedCellContainer & container;

    /**boundaries of the simulation: left, right, top, bottom(, front, back) */
    const std::array<double, 6> boundaries;

    /**Deletes particles that have left the boundaries of the simulation 
    */
    void handleOutflow();

    /**Reflects particles that are about to leave the boundaries of the simulation using ghost particles 
    */
    void handleReflecting();

   /**Particles that exit the domain on one side enter it from the opposite side*/
    void handlePeriodic();

   

};
