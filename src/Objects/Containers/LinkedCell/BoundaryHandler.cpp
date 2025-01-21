//
// Created by Stefanie Blattenberger on 27/11/2024.
//

#include "BoundaryHandler.h"
#include <math.h>
#include <exception>
#include <iostream>
#include <algorithm>
#include "spdlog/spdlog.h"
#include "utils/ArrayUtils.h"

#include "../../../Calculator/Calculator.h"
#include "../../../Calculator/LennardJonesCalculator.h"

Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator(false, false);

BoundaryHandler::BoundaryHandler(std::array<bCondition, 6> t,
                                 ParticleContainers::LinkedCellContainer &container) : type{t}, container{container},
    boundaries{{0, container.getDomainSize()[0], container.getDomainSize()[1], 0, container.getDomainSize()[2], 0}} {

    //initialize neighbors
    neighborCells.reserve(container.getBoundaryCells().size());

    for(auto cell:container.getBoundaryCells()){

        std::vector<std::pair<int, std::vector<int>>> neighbors;

        std::array <int, 3> cellPos = cell.get().getPosition();
        int cellIndex = container.cellIndex(cellPos[0], cellPos[1], cellPos[2]);

        for (int x = -1; x <= 1; x++){
            for (int y = -1; y <= 1; y++){
                for (int z = -1; z <= 1; z++){

                    if(x == 0 && y == 0 && z == 0){
                        continue;
                    }

                    if(container.getCellNumPerDimension()[2] == 1 && z != 0){ //for 2d
                        continue;
                    }

                    std::array<int,3> neighborPos = {cellPos[0] + x, cellPos[1] + y, cellPos[2] + z};
                    //Only add neighbor cell if requirements are met
                    //has to be halo cell of at least one boundary, if isHaloCell then corresponding boundary must be periodic

                    bool isHalo = false;
                    bool ifHaloThenPeriodic = true;

                    for (int i = 0; i < 6; i++){
                        if (isHaloCellofBoundary(i, neighborPos)){
                            isHalo = true;
                            if(!(type[i] == BoundaryHandler::bCondition::PERIODIC)){
                                ifHaloThenPeriodic = false;
                            }
                        }
                    }

                    if(isHalo && ifHaloThenPeriodic ){
                        //add cell
                        std::vector<int> directions; //direction in which particles will have to be temporarily moved during f calculation (opposite from the direction we are moving the cellposition to)
                        if(isHaloCellofBoundary(0, neighborPos)){
                            directions.push_back(1);
                        } else if(isHaloCellofBoundary(1, neighborPos)){
                            directions.push_back(0);
                        }

                        if(isHaloCellofBoundary(3, neighborPos)){
                            directions.push_back(2);
                        } else if(isHaloCellofBoundary(2, neighborPos)){
                            directions.push_back(3);
                        }

                        if(isHaloCellofBoundary(5, neighborPos)){
                            directions.push_back(4);
                        } else if(isHaloCellofBoundary(4, neighborPos)){
                            directions.push_back(5);
                        }

                        //find corresponding boundary cell on opposite side
                        //SPDLOG_INFO("position {} {} {}", neighborPos[0], neighborPos[1], neighborPos[2]);

                        for (auto dir:directions){
                            neighborPos = oppositeCell(neighborPos, oppositeSide(dir));
                             //SPDLOG_INFO("after {}: {} {} {}", dir, neighborPos[0], neighborPos[1], neighborPos[2]);
                        }

                        int neighborIndex = container.cellIndex(neighborPos[0], neighborPos[1], neighborPos[2]);
                         if(neighborIndex == -1){
                                SPDLOG_DEBUG("weird index, position {} {} {}", neighborPos[0], neighborPos[1], neighborPos[2]);
                            }

                        //make sure that every pair is only looked at once by comparing index

                        if(neighborIndex < cellIndex){
                            neighbors.push_back({neighborIndex, directions}); //add to neighbors of this cell
                            SPDLOG_DEBUG("added neighbor of {} {} {} : {} {} {}", cellPos[0], cellPos[1], cellPos[2], neighborPos[0], neighborPos[1], neighborPos[2]);
                        }

                    }      
            
        } //z
            
        } //y

        } //x

        neighborCells.push_back(neighbors);
    } //cells

    }


void BoundaryHandler::handleOutflow() {
    SPDLOG_TRACE("handle outflow");

    //check if we have at least one outflow boundary
    for (int i = 0; i < 6; i++){
        if(type[i] == bCondition::OUTFLOW){
            goto A; // continue with function
        }
    }

    return; // no outflow boundary found
    
    A:

        for (auto cell: container.getHaloCells()) {

            for (int i = 0; i < 6; i++) {
            //FOR 3D: 6

            if (type[i] != bCondition::OUTFLOW) { continue; }

            if (!isHaloCellofBoundary(i, cell.get().getPosition())) { continue; }

            for (auto p: cell.get().getParticlesInCell()) {
                container.removeParticle(*p);
                container.updateParticlesInCell();
            }
        }
    }
};


void BoundaryHandler::handleReflecting() {
    SPDLOG_TRACE("handle reflecting");

        for (auto cell: container.getBoundaryCells()) {

            for (int i = 0; i < 6; i++) {
        //0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 2 dimensions only for now

            if (type[i] != bCondition::REFLECTING) { continue; }

            if (!isBoundaryCellofBoundary(i, cell.get().getPosition())) { continue; }

            for (Particle *p: cell.get().getParticlesInCell()) {
                if (p->getFixed()) continue;
                //calculate distance from boundary
                double dist = calculateDistance(*p, i);
                if (dist < minDist(p->getSigma()) / 2) {
                    //must be closer
                    std::array<double, 3> sub = (ghostParticleLocation(*p, i, dist) - p->getX());
                    double norm = ArrayUtils::L2Norm(sub);
                    std::array<double, 3UL> force = calculator.calculateFIJ(
                        sub, 0, 0, norm, p->getEpsilon(), p->getEpsilon(), p->getSigma(), p->getSigma(),
                        false); //ghost particle has same epsilon and sigma
                    p->setF(p->getF() + force);
                }
            }
        }
    }
}

void BoundaryHandler::handlePeriodicMoveParticles(){
    for (auto cell : container.getHaloCells()){

        for (int i = 0; i < 6; i++){

            if(type[i] != BoundaryHandler::bCondition::PERIODIC){
                continue;
            }

            std::array<int,3> cellPos = cell.get().getPosition();
            if (isHaloCellofBoundary(i, cellPos)){
                moveParticlesToOppositeSideHelper(cell, cellPos, i);
                break; //perform move only once even for corner
            }
        }


    }
}

void BoundaryHandler::moveParticlesToOppositeSideHelper(Cell& cell, std::array<int,3> cellPos, int j){

    std::array<int,3> newCellPosition = oppositeCell(cellPos, j);
    Cell& newCell = container.getCells().at(container.cellIndex(newCellPosition[0], newCellPosition[1], newCellPosition[2]));

    for (auto p : cell.getParticlesInCell()){
        //move particles to opposite cell
        p->setX(cloneParticleLocation(p -> getX(), j));

        //adjust the particles in cells: avoid calling updateParticlesInCell because inefficient
        //add to new Cell 
        newCell.addParticleToCell(p);
    }

    //clear cell after all particles have been moved
    cell.clearParticlesInCell();

    //for corners: move again
    if(newCell.getCellType() == Cell::CType::HALO) {
        for (int i = 0; i < 6; i++){ 

            if(i == j || type[i] != BoundaryHandler::bCondition::PERIODIC){
                continue;
            }

            if (isHaloCellofBoundary(i, newCellPosition)){

                moveParticlesToOppositeSideHelper(newCell, newCellPosition, i);
                break; //perform move only once even for every cell even if it is a corner 
            }
        }
    }


}

void BoundaryHandler::handlePeriodicAddForces(){
    SPDLOG_TRACE("handle add forces");

    int size = (int) container.getBoundaryCells().size();
    
    for (int i = 0; i < size; i++){

        Cell& cell = container.getBoundaryCells().at(i);
        
        for (auto n : neighborCells.at(i)){
            
            Cell& neighborCell = container.getCells().at(n.first);

            //calculate force in directions
            for (auto neighborParticle : neighborCell.getParticlesInCell()){

                std::array<double, 3> tempLocation = neighborParticle -> getX();
                
                for (auto direction : n.second){ //temporarily move particle
                    tempLocation = cloneParticleLocation(tempLocation, direction);
                }

                for (auto particle:cell.getParticlesInCell()) {

                    //force between neighborP and P

                    std::array<double, 3> sub = particle-> getX() - tempLocation; //p2 - p1
                    double norm = ArrayUtils::L2Norm(sub);

                    if (norm > container.getCutoff()) {
                                    continue;
                                }

                    std::array<double,3> force = calculator.calculateFIJ(sub, 0, 0, norm, neighborParticle->getEpsilon(), particle -> getEpsilon(), neighborParticle -> getSigma(), particle -> getSigma(), true);
                    particle -> setF(particle -> getF() - force);
                    neighborParticle -> setF(neighborParticle -> getF() + force); 
                }
            }
        }
    }
}

double BoundaryHandler::calculateDistance(Particle p, int i) {
    //passing by value on purpose
    switch (i) {
        case 0: return std::abs(p.getX()[0] - boundaries[0]); //left
        case 1: return std::abs(p.getX()[0] - boundaries[1]); //right
        case 2: return std::abs(p.getX()[1] - boundaries[2]); //top
        case 3: return std::abs(p.getX()[1] - boundaries[3]); //bottom
        case 4: return std::abs(p.getX()[2] - boundaries[4]); //front
        case 5: return std::abs(p.getX()[2] - boundaries[5]); //back
        default: throw std::runtime_error("Error calculating distance");
    }
}

std::array<double, 3L> BoundaryHandler::ghostParticleLocation(Particle p, int i, double dist) {
    //pass by value
    std::array<double, 3> mirrorX = p.getX();

    switch (i) {
        case 0: {
            //mirror along left border
            mirrorX[0] = (mirrorX[0] - 2 * dist);
            break;
        }
        case 1: {
            //mirror along right border
            mirrorX[0] = (mirrorX[0] + 2 * dist);
            break;
        }
        case 2: {
            //mirror along top border
            mirrorX[1] = (mirrorX[1] + 2 * dist);
            break;
        }
        case 3: {
            //mirror along bottom border
            mirrorX[1] = (mirrorX[1] - 2 * dist);
            break;
        }
        case 4: {
            //mirror along front border
            mirrorX[2] = (mirrorX[2] + 2 * dist);
            //not sure yet if (0|0|0) is front bottom left or back bottom left corner
            break;
        }
        case 5: {
            //mirror along back border
            mirrorX[2] = (mirrorX[2] - 2 * dist);
            break;
        }
    }

    return mirrorX;
}

bool BoundaryHandler::isHaloCellofBoundary(int i, std::array<int, 3> pos) {
    switch (i) {
        case 0: return (pos[0] == -1);
        case 1: return (pos[0] == container.getCellNumPerDimension()[0]);
        case 2: return (pos[1] == container.getCellNumPerDimension()[1]);
        case 3: return (pos[1] == -1);
        case 4: if (container.getCellNumPerDimension()[2] == 1) { return false; } else return (
                pos[2] == container.getCellNumPerDimension()[2]);
        case 5: if (container.getCellNumPerDimension()[2] == 1) { return false; } else return (pos[2] == -1);
        default: throw std::runtime_error("Error calculating isHaloCellOfBoundary");
    }
}

bool BoundaryHandler::isBoundaryCellofBoundary(int i, std::array<int, 3> pos) {
    switch (i) {
        case 0: return (pos[0] == 0);
        case 1: return (pos[0] == container.getCellNumPerDimension()[0] - 1);
        case 2: return (pos[1] == container.getCellNumPerDimension()[1] - 1);
        case 3: return (pos[1] == 0);
        case 4: if (container.getCellNumPerDimension()[2] == 1) { return false; } else return (
                pos[2] == container.getCellNumPerDimension()[2] - 1);
        case 5: if (container.getCellNumPerDimension()[2] == 1) { return false; } else return (pos[2] == 0);
        default: throw std::runtime_error("Error calculating isBoundaryCellOfBoundary");
    }
}


std::array<double, 3UL> BoundaryHandler::cloneParticleLocation(std::array<double, 3UL> pos, int i) {
    switch (i) {
        case 0: {
            pos[0] += container.getDomainSize()[0];
            return pos;
        }
        case 1: {
            pos[0] -= container.getDomainSize()[0];
            return pos;
        }
        case 2: {
            pos[1] -= container.getDomainSize()[1];
            return pos;
        }
        case 3: {
            pos[1] += container.getDomainSize()[1];
            return pos;
        }
        case 4: {
            pos[2] -= container.getDomainSize()[2];
            return pos;
        }
        case 5: {
            pos[2] += container.getDomainSize()[2];
            return pos;
        }
        default: throw std::runtime_error("Error calculating cloneParticleLocation");
    }
};

double BoundaryHandler::minDist(double sigma) {
    return std::pow(2.0, 1.0 / 6.0) * sigma;
}


int BoundaryHandler::oppositeSide(int i) {
    switch (i) {
        case 0: return 1;
        case 1: return 0;
        case 2: return 3;
        case 3: return 2;
        case 4: return 5;
        case 5: return 4;
        default: return -1;
    }
}

std::array<int, 3> BoundaryHandler::oppositeCell(std::array<int, 3> position, int i) {
    switch (i) {
        case 0: position[0] = position[0] + container.getCellNumPerDimension()[0];
            break;
        case 1: position[0] = position[0] - container.getCellNumPerDimension()[0];
            break;
        case 2: position[1] = position[1] - container.getCellNumPerDimension()[1];
            break;
        case 3: position[1] = position[1] + container.getCellNumPerDimension()[1];
            break;
        case 4: position[2] = position[2] - container.getCellNumPerDimension()[2];
            break;
        case 5: position[2] = position[2] + container.getCellNumPerDimension()[2];
            break;
        default: throw std::runtime_error("Error calculating oppositeCell");
    }

    return position;
}

std::array<int, 3> BoundaryHandler::oppositeNeighborCell(std::array<int, 3> position, int i) {
    switch (i) {
        case 0: position[0] = position[0] + container.getCellNumPerDimension()[0] - 1;
            break;
        case 1: position[0] = position[0] - container.getCellNumPerDimension()[0] + 1;
            break;
        case 2: position[1] = position[1] - container.getCellNumPerDimension()[1] + 1;
            break;
        case 3: position[1] = position[1] + container.getCellNumPerDimension()[1] - 1;
            break;
        case 4: position[2] = position[2] - container.getCellNumPerDimension()[2] + 1;
            break;
        case 5: position[2] = position[2] + container.getCellNumPerDimension()[2] - 1;
            break;
        default: throw std::runtime_error("Error calculating oppositeCell");
    }

    return position;
}

