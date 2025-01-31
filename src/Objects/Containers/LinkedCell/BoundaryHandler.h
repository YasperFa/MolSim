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

     /**Get opposite boundary cell on opposite side for a halo cell / 
      * Get opposite halo cell on opposite side for a boundary cell / 
     * @param position position of input cell in the cell grid
     * @param i boundary that input cell borders on
     * @returns position of cell on opposite side bordering on opposite(i)
     */
    
    std::array<int, 3> oppositeCell(std::array<int, 3> position, int i);


     /**Deletes particles that have left the boundaries of the simulation 
    */
    void handleOutflow();

    /**Reflects particles that are about to leave the boundaries of the simulation using ghost particles 
    */
    void handleReflecting();

    /**Adds forces of particles in boundary cells on the opposite side of the domain to particles in boundary cells if necessary */
    void handlePeriodicAddForces();

    /**Moves particles in halo cells to boundary cells on the opposite side of the domain */
    void handlePeriodicMoveParticles();

    private:

    /**For periodic boundary handling
     * Contains for every boundary cell a vector with entries that each represent a boundary cell on the opposite side of the domain which has to be included for force calculations
     * Every entry consists of a pair of
     * - Reference to the cell
     * - vector containing up to 3 ints which show how the position of the neighbor cell relates to the position of the cell
     * 
     * Every pair of boundary cells is only represented once in the vector
     */
    std::vector<std::vector<std::pair<std::reference_wrapper<Cell>, std::vector<int>>>> neighborCells;

    /** determines what condition is used on what border
     * boundaries of the simulation: left, right, top, bottom, front, back
     */
    const std::array<bCondition, 6> type; 

    /** ParticleContainer that the BoundaryHandler operates on */
    ParticleContainers::LinkedCellContainer & container;

    /**boundaries of the simulation: left, right, top, bottom, front, back*/
    const std::array<double, 6> boundaries;

    /**Get int of opposite side
     * @param i input side
     * @returns opposite side
     */
    int oppositeSide(int i);

    /** Helper function for moving all particles of a halo cell to the boundary cell on the opposite side of the domain
     * @param cell Reference to the cell
     * @param cellPos position of the cell in the cell grid
     * @param i boundary that the halo cell borders on
    */
    void moveParticlesToOppositeSideHelper(Cell & cell, std::array<int, 3> cellPos, int i);

    /** Get boundary cell on opposite side of the domain for a given boundary cell
     * @param position Position of the cell in the cell grid
     * @param i Boundary that the cell borders on
     * @returns position of corresponding boundary cell that borders on opposite(i)
     */
    std::array<int, 3> oppositeNeighborCell(std::array<int, 3> position, int i);

};
