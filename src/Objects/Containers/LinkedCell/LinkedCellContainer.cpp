//
// Created by Celik Yucel on 26.11.24.
//

#include "LinkedCellContainer.h"

#include "spdlog/spdlog.h"

namespace ParticleContainers {
    LinkedCellContainer::LinkedCellContainer(const std::array<double, 3> &domainSize, const double cutoff)
        : domainSize(domainSize), cutoff(cutoff) {

        SPDLOG_DEBUG("DOMAIN SIZE: {} {} {}", domainSize[0], domainSize[1], domainSize[2]);
        SPDLOG_DEBUG("CUTOFF: {}", cutoff);

        //calculate how many cells in each dimension
        cellNumPerDimension = {
            std::max(static_cast<int>(std::floor(domainSize[0] / cutoff)), 1),
            std::max(static_cast<int>(std::floor(domainSize[1] / cutoff)), 1),
            1 // for now 2d
        };

        SPDLOG_DEBUG("Cell Number Per Dimension: {} {} {}", cellNumPerDimension[0], cellNumPerDimension[1], cellNumPerDimension[2]);

        //calculate the sizes for cells
        cellSizePerDimension = {
            domainSize[0] / cellNumPerDimension[0], domainSize[1] / cellNumPerDimension[1],
            domainSize[2] / cellNumPerDimension[2]
        };

        cells.reserve((cellNumPerDimension[0] + 2) * (cellNumPerDimension[1] + 2)); //
        SPDLOG_DEBUG("Cell Size Per Dim {} {} {}", cellSizePerDimension[0], cellSizePerDimension[1], cellSizePerDimension[2]);


        //initialize cells with the correct cell types
        initializeCells();

        //initialize the neighbours vectors for the cells
        initializeNeighbours();

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
        //SPDLOG_INFO(particle.toString());
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
            SPDLOG_WARN("Cell does not exist, particle is out of bounds!");
            return;
        }
    }

    void LinkedCellContainer::removeParticle(const Particle &particle) {
        SPDLOG_DEBUG("removing {}", particle.toString());
        Cell *cellOfParticle = mapParticleToCell(particle);
        if (cellOfParticle != nullptr) {

            particles.erase(std::find(particles.begin(), particles.end(), particle));

        } else {
            SPDLOG_ERROR("Cell does not exist, particle to be removed is out of bounds!");
            throw std::runtime_error("Cell does not exist, particle to be removed is out of bounds!");
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
            if (cell != nullptr) {
                cell->addParticleToCell(&particle);
            } else {
                auto it = std::find(particles.begin(), particles.end(), particle); // causes container overflow on address sanitizer
                if (it != particles.end()) {
                    particles.erase(it);
                    SPDLOG_INFO("Particle erased successfully.");
                } else {
                    SPDLOG_WARN("Particle not found in container.");
                }
            SPDLOG_DEBUG("removed particle out of bounds");
           }
        }
    }

    int LinkedCellContainer::cellIndex(int x, int y, int z) const {

        const int numCellsInXDim = cellNumPerDimension[0];
        const int numCellsInYDim = cellNumPerDimension[1];
        const int numCellsInZDim = 1;
        SPDLOG_DEBUG("x {} y {} z {} {} {} {} {} ", x, y, z, numCellsInXDim, numCellsInYDim, numCellsInZDim, cellSizePerDimension[2]);

        if (x < -1 || y < -1 || z < -1 || x > (numCellsInXDim) || y > (numCellsInYDim) || z > (numCellsInZDim)) { //flying out too far
            //SPDLOG_ERROR("x {} y {} z {} {} {} {} {} ", x, y, z, numCellsInXDim, numCellsInYDim, numCellsInZDim, cellSizePerDimension[2]);
            return -1;
        }

        const int strideYZ = (numCellsInYDim + 2)*1;
        const int strideZ = 1; // for 2d
        return (x + 1) * strideYZ + (y + 1) * strideZ; // for 2d
    }


    Cell *LinkedCellContainer::mapParticleToCell(const Particle &particle) {
        std::array<double, 3> particlePosition = particle.getX();
        std::array<int, 3> cellPosition = {
            static_cast<int>(std::floor(particlePosition[0] / cellSizePerDimension[0])),
            static_cast<int>(std::floor(particlePosition[1] / cellSizePerDimension[1])),
            static_cast<int>(std::floor(particlePosition[2] / cellSizePerDimension[2]))
        };
        //SPDLOG_INFO("{} {}", particlePosition[0], cellSizePerDimension[0] );
        int cellInd = cellIndex(cellPosition[0], cellPosition[1], cellPosition[2]);
        if (cellInd >= cells.size() || cellInd < 0) {
            SPDLOG_WARN("The given particle does not belong to any cell!");
            return nullptr;
        }

        return &cells.at(cellInd);
    }

    void LinkedCellContainer::initializeCells() {
        SPDLOG_DEBUG("Initializing cells...");
        for (int x = -1; x < cellNumPerDimension[0] + 1; ++x) {
            for (int y = -1; y < cellNumPerDimension[1] + 1 ; ++y) {
                    if (x < 0 || y < 0 ||  x >= cellNumPerDimension[0] || y >= cellNumPerDimension[1]) {
                        Cell nCell(Cell::CType::HALO);
                        cells.push_back(nCell);
                        haloCells.push_back(cells.back());
                    } else if (x == 0 || y == 0 || x == cellNumPerDimension[0] - 1 || y == cellNumPerDimension[1] - 1) {
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
        updateParticlesInCell();
    }

    void LinkedCellContainer::initializeNeighbours() {
         SPDLOG_DEBUG("Initializing neighbours...");
        const int numCellsInXDim = cellNumPerDimension[0];
        const int numCellsInYDim = cellNumPerDimension[1];
        for (int x = -1; x < numCellsInXDim + 1; ++x) {
            for (int y = -1; y < numCellsInYDim + 1; ++y) {
                    int z = 0; // for 2d
                    Cell& cell = cells.at(cellIndex(x, y, z));
                    for (int neighbourX = -1; neighbourX <= 1; ++neighbourX) {
                        for (int neighbourY = -1; neighbourY <= 1; ++neighbourY) {
                                if (neighbourX == 0 && neighbourY == 0) {
                                    continue;
                                }
                                int neighbourIndex = cellIndex(x + neighbourX, y + neighbourY, z);
                                if (neighbourIndex == -1) {
                                    continue;
                                }
                                cell.addNeighbourCell(&cells.at(neighbourIndex));

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