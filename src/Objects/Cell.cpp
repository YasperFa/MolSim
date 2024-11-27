//
// Created by Celik Yucel on 26.11.24.
//

#include "Cell.h"
Cell::Cell(CType cellType) : cellType(cellType) {}

std::vector<Particle*>::iterator Cell::beginInCell() {
    return particlesInCell.begin();
}
std::vector<Particle*>::iterator Cell::endInCell() {
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
    influencedBy.insert(cell);
}

std::vector<Particle*> Cell::getParticlesInCell(){return particlesInCell;}

std::vector<Cell*> Cell::getNeighbourCells(){return neighbourCells;}

Cell::CType Cell::getCellType(){ return cellType;}



