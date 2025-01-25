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

     /**Get opposite boundary cell on opposite side for a halo cell
     * @param position position of the cell in the cell grid
     * @param i boundary of halo cell
     * @returns position of boundary cell on opposite side
     */
    
    std::array<int, 3> oppositeCell(std::array<int, 3> position, int i);


     /**Deletes particles that have left the boundaries of the simulation 
    */
    void handleOutflow();

    /**Reflects particles that are about to leave the boundaries of the simulation using ghost particles 
    */
    void handleReflecting();

    void handlePeriodicAddForces();

    void handlePeriodicMoveParticles();

    private:

    std::vector<std::vector<std::pair<std::reference_wrapper<Cell>, std::vector<int>>>> neighborCells;

    /** determines what condition is used on what border
     * boundaries of the simulation: left, right, top, bottom(, front, back)
     */
    const std::array<bCondition, 6> type; 

    /** ParticleContainer that the BoundaryHandler operates on */
    ParticleContainers::LinkedCellContainer & container;

    /**boundaries of the simulation: left, right, top, bottom(, front, back) */
    const std::array<double, 6> boundaries;

    /**Get int of opposite side
     * @param i input side
     * @returns opposite side
     */
    int oppositeSide(int i);

    void moveParticlesToOppositeSideHelper(Cell & cell, std::array<int, 3> cellPos, int i);

    std::array<int, 3> oppositeNeighborCell(std::array<int, 3> position, int i);

};
