//
// Created by Celik Yucel on 26.11.24.
//

#include "Cell.h"

#include "spdlog/spdlog.h"
Cell::Cell(CType cellType, std::array <int, 3> pos) : cellType(cellType), position(pos){}

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

std::array<int, 3> Cell::getPosition(){return position;};





