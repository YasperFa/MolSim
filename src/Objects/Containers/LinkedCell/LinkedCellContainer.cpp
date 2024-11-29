//
// Created by Celik Yucel on 26.11.24.
//

#include "LinkedCellContainer.h"

#include "spdlog/spdlog.h"

namespace ParticleContainers {
    LinkedCellContainer::LinkedCellContainer(const std::array<double, 3> &domainSize, const double cutoff)
        : domainSize(domainSize), cutoff(cutoff) {

        SPDLOG_INFO("DOMAIN SIZE: {} {} {}", domainSize[0], domainSize[1], domainSize[2]);
        SPDLOG_INFO("CUTOFF: {}", cutoff);

        //calculate how many cells in each dimension
        cellNumPerDimension = {
            std::max(static_cast<int>(std::floor(domainSize[0] / cutoff)), 1),
            std::max(static_cast<int>(std::floor(domainSize[1] / cutoff)), 1),
            std::max(static_cast<int>(std::floor(domainSize[2] / cutoff)), 1)
        };

        SPDLOG_INFO("Cell Number Per Dimension: {} {} {}", cellNumPerDimension[0], cellNumPerDimension[1], cellNumPerDimension[2]);

        //calculate the sizes for cells
        cellSizePerDimension = {
            domainSize[0] / cellNumPerDimension[0], domainSize[1] / cellNumPerDimension[1],
            domainSize[2] / cellNumPerDimension[2]
        };

        SPDLOG_INFO("Cell Size Per Dim {} {} {}", cellSizePerDimension[0], cellSizePerDimension[1], cellSizePerDimension[2]);


        SPDLOG_INFO("LinkedCellContainer initialized1");
        //initialize cells with the correct cell types
        initializeCells();
        SPDLOG_INFO("LinkedCellContainer initialized2");

        //initialize the neighbours vectors for the cells
        initializeNeighbours();
        SPDLOG_INFO("LinkedCellContainer initialized3");

        reserve(3000);

        SPDLOG_DEBUG("LinkedCellContainer initialized with the domain [{},{},{}] and cutoff-radius", domainSize[0],
                     domainSize[1], domainSize[2], cutoff);
        SPDLOG_DEBUG("Number of cells per dimension: x: {}, y: {}, z: {}", cellNumPerDimension[0], cellNumPerDimension[1],
                     cellNumPerDimension[2]);
        SPDLOG_DEBUG("Cell size per dimension: x: {}, y: {}, z: {}", cellSizePerDimension[0], cellSizePerDimension[1],
                     cellSizePerDimension[2]);
        SPDLOG_INFO("{} inner cells, {} boundary cells and {} halo cells", innerCells.size(), boundaryCells.size(),
                     haloCells.size());
    }

    void LinkedCellContainer::addParticle(const Particle &particle) {
        Cell *cellOfParticle = mapParticleToCell(particle);
        if (cellOfParticle != nullptr) {

            size_t cap = particles.capacity();
            particles.push_back(particle);

            if (cap != particles.capacity()) {
                updateParticlesInCell();
            } else {
                cellOfParticle->addParticleToCell(&particles.back());
            }
        } else {
            SPDLOG_ERROR("Cell does not exist, particle is out of bounds!");
            throw std::runtime_error("Cell does not exist, particle is out of bounds!");
        }
    }

    void LinkedCellContainer::reserve(size_t numParticles) {
        size_t oldCap = particles.capacity();
        particles.reserve(numParticles);

        if (oldCap != particles.capacity()) {
            updateParticlesInCell();
        }
    }

    void LinkedCellContainer::updateParticlesInCell() {
        for (Cell& cell : cells) {
            cell.clearParticlesInCell();
        }

        for (Particle& particle : particles) {
            Cell* cell = mapParticleToCell(particle);
            cell->addParticleToCell(&particle);
        }
    }

    int LinkedCellContainer::cellIndex(int x, int y, int z) const {

        const int numCellsInXDim = cellNumPerDimension[0];
        const int numCellsInYDim = cellNumPerDimension[1];
        const int numCellsInZDim = cellNumPerDimension[2];

        if (x < -1 || y < -1 || z < -1 || x > numCellsInXDim || y > numCellsInYDim || z > numCellsInZDim) {
            return -1;
        }

        const int strideYZ = (numCellsInYDim + 2) * (numCellsInZDim + 2);
        const int strideZ = (numCellsInZDim + 2);
        return (x + 1) * strideYZ + (y + 1) * strideZ + (z + 1);
    }


    Cell *LinkedCellContainer::mapParticleToCell(const Particle &particle) {
        std::array<double, 3> particlePosition = particle.getX();
        std::array<int, 3> cellPosition = {
            static_cast<int>(std::floor(particlePosition[0] / cellSizePerDimension[0])),
            static_cast<int>(std::floor(particlePosition[1] / cellSizePerDimension[1])),
            static_cast<int>(std::floor(particlePosition[2] / cellSizePerDimension[2]))
        };
        int cellInd = cellIndex(cellPosition[0], cellPosition[1], cellPosition[2]);
        if (cellInd == -1) {
            SPDLOG_ERROR("The given particle does not belong to any cell!");
            throw std::runtime_error("The given particle is not in the cell!");
        }

        return &cells.at(cellInd);
    }

    void LinkedCellContainer::initializeCells() {
        SPDLOG_DEBUG("Initializing cells...");
        for (int x = -1; x < cellNumPerDimension[0]; ++x) {
            for (int y = -1; y < cellNumPerDimension[1]; ++y) {
                for (int z = -1; z < cellNumPerDimension[2]; ++z) {
                    if (x < 0 || y < 0 || z < 0 || x >= cellNumPerDimension[0] || y >= cellNumPerDimension[1] || z >= cellNumPerDimension[2]) {
                        Cell nCell(Cell::CType::HALO);
                        cells.push_back(nCell);
                        haloCells.push_back(cells.back());
                    } else if (x == 0 && y == 0 && z == 0 || x == cellNumPerDimension[0] - 1 || y == cellNumPerDimension[1] - 1 || z == cellNumPerDimension[2] - 1) {
                        Cell nCell(Cell::CType::BOUNDARY);
                        cells.push_back(nCell);
                        boundaryCells.push_back(cells.back());
                        innerCells.push_back(cells.back());
                    } else {
                        Cell nCell(Cell::CType::INNER);
                        cells.push_back(nCell);
                        innerCells.push_back(cells.back());
                    }
                }
            }
        }
    }

    void LinkedCellContainer::deleteHaloParticles() {
        std::unordered_set<Particle *> particlesDelete;
        for (auto &cell: haloCells) {
            for (Particle *particle: cell.get().getParticlesInCell()) {
                particlesDelete.insert(particle);
            }
        }

        auto isInDelete = [&](Particle &particle) {
            return particlesDelete.find(&particle) != particlesDelete.end();
        };

        particles.erase(std::remove_if(particles.begin(), particles.end(), isInDelete), particles.end());
    }

    void LinkedCellContainer::initializeNeighbours() {
        const int numCellsInXDim = cellNumPerDimension[0];
        const int numCellsInYDim = cellNumPerDimension[1];
        const int numCellsInZDim = cellNumPerDimension[2];

        for (int x = -1; x < numCellsInXDim + 1; ++x) {
            for (int y = -1; y < numCellsInYDim + 1; ++y) {
                for (int z = -1; z < numCellsInZDim + 1; ++z) {
                    Cell& cell = cells.at(cellIndex(x, y, z));

                    for (int neighbourX = -1; neighbourX <= 2; ++neighbourX) {
                        for (int neighbourY = -1; neighbourY <= 2; ++neighbourY) {
                            for (int neighbourZ = -1; neighbourZ <= 2; ++neighbourZ) {
                                if (neighbourX == 0 && neighbourY == 0 && neighbourZ == 0) {
                                    continue;
                                }

                                int neighbourIndex = cellIndex(x + neighbourX, y + neighbourY, z + neighbourZ);
                                if (neighbourIndex == -1) {
                                    continue;
                                }

                                cell.addNeighbourCell(&cells.at(neighbourIndex));
                            }
                        }
                    }
                }
            }
        }
    }


    std::vector<Particle>::iterator LinkedCellContainer::begin() { return particles.begin(); }

    std::vector<Particle>::iterator LinkedCellContainer::end() { return particles.end(); }

    std::vector<Cell>::iterator LinkedCellContainer::beginCells() { return cells.begin(); }

    std::vector<Cell>::iterator LinkedCellContainer::endCells() { return cells.end(); }

    std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::beginInnerCells() {
        return innerCells.begin();
    }

    std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::endInnerCells() { return innerCells.end(); }

    std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::beginBoundaryCells() {
        return boundaryCells.begin();
    }

    std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::endBoundaryCells() {
        return boundaryCells.end();
    }

    std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::beginHaloCells() { return haloCells.begin(); }

    std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::endHaloCells() { return haloCells.end(); }

    std::vector<Particle> LinkedCellContainer::getParticles() const { return particles; }

    size_t LinkedCellContainer::sizeParticles() const { return particles.size(); }

    std::vector<Cell> LinkedCellContainer::getCells() const { return cells; }

    size_t LinkedCellContainer::sizeCells() const { return cells.size(); }

    std::vector<std::reference_wrapper<Cell> > LinkedCellContainer::getInnerCells() { return innerCells; }

    std::vector<std::reference_wrapper<Cell> > LinkedCellContainer::getBoundaryCells() { return boundaryCells; }

    std::vector<std::reference_wrapper<Cell> > LinkedCellContainer::getHaloCells() { return haloCells; }

    double LinkedCellContainer::getCutoff() const { return cutoff; }

    std::array<double, 3> LinkedCellContainer::getDomainSize() const { return domainSize; }

    std::array<double, 3> LinkedCellContainer::getCellSizePerDimension() const { return cellSizePerDimension; }

    std::array<int, 3> LinkedCellContainer::getCellNumPerDimension() const { return cellNumPerDimension; }
}