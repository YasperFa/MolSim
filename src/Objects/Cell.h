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
    enum CellType { INNER, HALO, BORDER };

private:
    /**
     * @brief private parameter for the cell type
     */
    CellType celltype;

    /**
     * @brief A vector containing references to particles that are withing the cell
     */
    std::vector<std::reference_wrapper<Particle> > ParticlesInCell;

    /**
     * @brief A vector containing references to the current cell's neighbouring cells
     */
    std::vector<std::reference_wrapper<Cell> > NeighbourCells;

    /**
     * @brief A set of Cell object references. The cells in this set have influenced the current cell.
     */
    std::unordered_set<std::reference_wrapper<Cell> > InfluencedBy;

public:
    /**
     * @brief constructor for a Cell object
     * @param cellType determines the type of the Cell object that is being created.
     */
    explicit Cell(CellType cellType);
};
