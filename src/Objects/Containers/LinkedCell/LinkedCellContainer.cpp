//
// Created by Celik Yucel on 26.11.24.
//

#include "LinkedCellContainer.h"

#include "spdlog/spdlog.h"

std::vector<Particle>::iterator LinkedCellContainer::beginParticles() { return particles.begin(); }

std::vector<Particle>::iterator LinkedCellContainer::endParticles() { return particles.end(); }

std::vector<Cell>::iterator LinkedCellContainer::beginCells() { return cells.begin(); }

std::vector<Cell>::iterator LinkedCellContainer::endCells() { return cells.end(); }

std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::beginInnerCells() { return innerCells.begin(); }

std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::endInnerCells() { return innerCells.end(); }

std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::beginBoundaryCells() { return boundaryCells.begin(); }

std::vector<std::reference_wrapper<Cell> >::iterator LinkedCellContainer::endBoundaryCells() { return boundaryCells.end(); }

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
