//
// Created by Celik Yucel on 24.11.24.
//

#pragma once
#include <unordered_set>
#include <vector>

#include "Particle.h"

class Cell {
public:
    /**
     * Enum to define the different cell types
     */
    enum class CType {
     INNER,
     HALO,
     BOUNDARY
    };

private:
    /**
     * @brief private parameter for the cell type
     */
    CType cellType;

    /**
     * @brief A vector containing references to particles that are withing the cell
     */
    std::vector<Particle*> particlesInCell;

    /**
     * @brief A vector containing references to the current cell's neighbouring cells
     */
    std::vector<Cell*> neighbourCells;

    /**
     * @brief A set of Cell object references. The cells in this set have influenced the current cell.
     */
    std::unordered_set<Cell*> influencedBy;

public:
    /**
     * @brief constructor for a Cell object
     * @param cellType determines the type of the Cell object that is being created.
     */
    explicit Cell(CType cellType);

    /**
     *
     * @return an iterator to the beginning of the vector containing raw pointers to particles in the cell
     */
    std::vector<Particle*>::iterator beginParticle();

    /**
     *
     * @return an iterator to the end of the vector containing raw pointers to particles in the cell
     */
    std::vector<Particle*>::iterator endParticle();

   /**
    *
    * @return an iterator to the beginning of the vector containing pointers to neighbouring cell
    */
    std::vector<Cell*>::iterator beginNeighbourCell();

   /**
    *
    * @return an iterator to the end of the vector containing pointers to neighbouring cell
    */
    std::vector<Cell*>::iterator endNeighbourCell();

    /**
    *
    * @return an iterator to the beginning of the set containing the cells that have already affected the current cell
    */
    std::unordered_set<Cell*>::iterator beginInfluencedBy();

   /**
    *
    * @return an iterator to the end of the set containing the cells that have already affected the current cell
    */
    std::unordered_set<Cell*>::iterator endInfluencedBy();

    /**
     * Add a particle reference to the cell.
     * @param particle is the particle to be added
     */
    void addParticleToCell(Particle* particle);


    /**
     * Remove a particle from the cell
     * @param particle is the particle to be removed
     */
    void removeParticleFromCell(const Particle & particle);

    /**
     * Add the passed cell as a reference to the NeighbourCells vector.
     * @param cell is the cell to be added as a neighbour
     */
    void addNeighbourCell(Cell* cell);

    /**
     * Add the passed cell as a reference to the influencedBy vector.
     * @param cell is the cell to be added as an influencer
     */
    void addInfluencedByCell(Cell* cell);

    /**
     * @brief clear the particlesInCell vector for the current cell.
     */
    void clearParticlesInCell();

    std::vector<Particle*> getParticlesInCell();

    std::vector<Cell*> getNeighbourCells();

    std::unordered_set<Cell*> getInfluencedByCells();

    CType getCellType();
};
