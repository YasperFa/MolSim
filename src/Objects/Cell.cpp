//
// Created by Celik Yucel on 26.11.24.
//

#include "Cell.h"

#include "spdlog/spdlog.h"
Cell::Cell(CType cellType) : cellType(cellType) {}

std::vector<Particle*>::iterator Cell::beginParticle() {
    return particlesInCell.begin();
}

std::vector<Particle*>::iterator Cell::endParticle() {
    return particlesInCell.end();
}

std::vector<Cell*>::iterator Cell::beginNeighbourCell() {
    return neighbourCells.begin();
}

std::vector<Cell*>::iterator Cell::endNeighbourCell() {
    return neighbourCells.end();
}

std::unordered_set<Cell*>::iterator Cell::beginInfluencedBy() {
    return influencedBy.begin();
}

std::unordered_set<Cell*>::iterator Cell::endInfluencedBy() {
    return influencedBy.end();
}

void Cell::addParticleToCell(Particle* particle) {
    particlesInCell.push_back(particle);
}

void Cell::removeParticleFromCell(const Particle & particle) {
       SPDLOG_TRACE("removing particle from cell");

        // remove the particle itself from `particles`
        auto it = std::remove_if(
            particlesInCell.begin(),
            particlesInCell.end(),
            [&particle](Particle * p) { return p->getID() == particle.getID(); });

        if (it != particlesInCell.end()) {
            particlesInCell.erase(it, particlesInCell.end());
        } else {
            SPDLOG_WARN("Attempted to remove a particle not found in cell: ID {}", particle.getID());
        }
    }

void Cell::addNeighbourCell(Cell* cell) {
    neighbourCells.push_back(cell);
}

void Cell::addInfluencedByCell(Cell* cell) {
    if (cell == nullptr) {
        SPDLOG_DEBUG("Cell::addInfluencedByCell(): cell is null");
        return;
    }
    influencedBy.insert(cell);
}

std::vector<Particle*> Cell::getParticlesInCell(){return particlesInCell;}

std::vector<Cell*> Cell::getNeighbourCells(){return neighbourCells;}

Cell::CType Cell::getCellType(){ return cellType;}

std::unordered_set<Cell *> Cell::getInfluencedByCells() {
    return influencedBy;
}

void Cell::clearParticlesInCell() {
    particlesInCell.clear();
}





