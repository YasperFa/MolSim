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
     BORDER
    };

private:
    /**
     * @brief private parameter for the cell type
     */
    CType cellType;

    /**
     * @brief A vector containing references to particles that are withing the cell
     */
    std::vector<std::reference_wrapper<Particle>> ParticlesInCell;

    /**
     * @brief A vector containing references to the current cell's neighbouring cells
     */
    std::vector<std::reference_wrapper<Cell>> NeighbourCells;

    /**
     * @brief A set of Cell object references. The cells in this set have influenced the current cell.
     */
    std::unordered_set<std::reference_wrapper<Cell>> InfluencedBy;

public:
    /**
     * @brief constructor for a Cell object
     * @param cellType determines the type of the Cell object that is being created.
     */
    explicit Cell(CType cellType);

    /**
     *
     * @return an iterator to the beginning of the vector containing the particle references in the cell
     */
    std::vector<std::reference_wrapper<Particle>>::iterator beginInCell();

    /**
     *
     * @return an iterator to the end of the vector containing the particle references in the cell
     */
    std::vector<std::reference_wrapper<Particle>>::iterator endInCell();

   /**
    *
    * @return an iterator to the beginning of the vector containing the neighbouring cell references
    */
    std::vector<std::reference_wrapper<Cell>>::iterator beginNeighbourCell();

   /**
    *
    * @return an iterator to the end of the vector containing the neighbouring cell references
    */
    std::vector<std::reference_wrapper<Cell>>::iterator endNeighbourCell();

    /**
    *
    * @return an iterator to the beginning of the set containing the cells that have already affected the current cell
    */
    std::unordered_set<std::reference_wrapper<Cell>>::iterator beginInfluencedBy();

   /**
    *
    * @return an iterator to the end of the set containing the cells that have already affected the current cell
    */
    std::unordered_set<std::reference_wrapper<Cell>>::iterator endInfluencedBy();

    /**
     * Add a particle reference to the cell
     * @param particle is the particle to be added
     */
    void addParticleToCell(Particle& particle);

    /**
     * Add the passed cell as a reference to the NeighbourCells vector
     * @param cell is the cell to be added as a neighbour
     */
    void addNeighbourCell(Cell& cell);

};
