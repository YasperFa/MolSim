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

Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator();

BoundaryHandler::BoundaryHandler(std::array<bCondition, 6> t, ParticleContainers::LinkedCellContainer& container) :
type {t}, container {container}, 
boundaries {{0, container.getDomainSize()[0], container.getDomainSize()[1], 0, container.getDomainSize()[2], 0}} {
};


void BoundaryHandler::handleBoundaries(){
            handlePeriodic();
            handleReflecting();
            handleOutflow();
            updatePartners();
    }


void BoundaryHandler::handleOutflow(){
    SPDLOG_TRACE("handle outflow");

    for (int i = 0; i < 6; i++) { //FOR 3D: 6

     if (type[i] != bCondition::OUTFLOW) {continue;}

        for (auto cell : container.getHaloCells()) {

            if (!isHaloCellofBoundary(i, cell.get().getPosition())){continue;}

            for (auto p : cell.get().getParticlesInCell()) {

                container.removeParticle(*p);
                container.updateParticlesInCell();
            }

        }
       
    } 
  
};

void BoundaryHandler::handleReflecting(){

    SPDLOG_TRACE("handle reflecting");

    for (int i = 0; i < 6; i++) {//0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 2 dimensions only for now

        if (type [i] != bCondition::REFLECTING) {continue;}
   
        for (auto cell : container.getBoundaryCells()){

            if (!isBoundaryCellofBoundary(i, cell.get().getPosition())) {continue;}
        
            for (Particle * p : cell.get().getParticlesInCell()) {
  
                //calculate distance from boundary
                double dist = calculateDistance(*p, i);
                if (dist < minDist(p -> getSigma())/2) { //must be closer
                    std::array<double, 3> sub = (ghostParticleLocation(*p, i, dist) - p->getX());
                    double norm = ArrayUtils::L2Norm(sub);
                    std::array <double, 3UL> force = calculator.calculateFIJ(sub, 0, 0, norm, p-> getEpsilon(), p-> getEpsilon(), p -> getSigma(), p-> getSigma()); //ghost particle has same epsilon and sigma
                    p->setF(p -> getF() + force);
                    
                }
            }
        }

        for (auto cell : container.getHaloCells()){

            if (!isBoundaryCellofBoundary(i, cell.get().getPosition())) {continue;}
        
            for (Particle * p : cell.get().getParticlesInCell()) {
  
                //calculate distance from boundary
                double dist = calculateDistance(*p, i);
                if (dist < minDist(p -> getSigma())/2) { //must be closer
                    std::array<double, 3> sub = (ghostParticleLocation(*p, i, dist) - p->getX());
                    double norm = ArrayUtils::L2Norm(sub);
                    std::array <double, 3UL> force = calculator.calculateFIJ(sub, 0, 0, norm, p-> getEpsilon(), p-> getEpsilon(), p -> getSigma(), p-> getSigma()); //ghost particle has same epsilon and sigma
                    p->setF(p -> getF() + force);
                    
                }
            }
        }
    }    
     
}

void BoundaryHandler::handlePeriodic() {


    for (auto cell:container.getHaloCells()){ //clones of existing clones if necessary

    std::array <int,3> cellPos = cell.get().getPosition();

        for (int j = 0; j < 6; j++) {

        if (type [j] != bCondition::PERIODIC || !isBoundaryCellofBoundary(j, cellPos)){
                        continue;
        }

            for (auto particle : cell.get().getParticlesInCell()) {
                SPDLOG_INFO("checking existing clone with id {} in direction {}", particle -> getID(), j);
                
                cloneOfCloneHelper(*particle, cellPos, -1, j, true);
            }
        }
        }



    for (int i = 0; i < 6; i++) { //create clones of particles in boundary cells

        if (type [i] != bCondition::PERIODIC) {continue;}

        for (auto cell : container.getBoundaryCells()){
            if (!isBoundaryCellofBoundary(i, cell.get().getPosition())) {continue;}

            for (auto particle : cell.get().getParticlesInCell()) {
                
                if (!movedIntoBoundary(i, particle -> getOldX())) {continue;} //only create clone particle if particle moved into boundary in this iteration
                Particle newParticle = createCloneParticle(i, *particle);
                std::array<int, 3> cellPos = container.mapParticleToCell(newParticle)->getPosition();
                newParticle.setPartner(i, particle -> getID());
                particle -> setPartner(i, newParticle.getID()); //if newParticle moves into domain and particle leaves domain, they switch roles

                for (int j = 0; j < 6; j++){
                cloneOfCloneHelper(newParticle, cellPos, i, j, true); //clones of new clones if necessary
                }

                container.addParticle(newParticle);

            }
        }
        
    }

    
}

void BoundaryHandler::cloneOfCloneHelper(Particle& currentParticle, std::array <int, 3> cellPos, int i, int j, bool cont){

                    if (j == oppositeSide(i) || j == i || type [j] != bCondition::PERIODIC || !isBoundaryCellofBoundary(j, cellPos)){
                        return;
                    }
                    if (!movedIntoBoundary(j, currentParticle.getOldX())) {return;} //only create clone particle if particle moved into boundary in this iteration

                    //check if particle already exists, set partner if yes, else create new particle
                    std::array<int, 3> newCellPos = oppositeCell(cellPos, j);

                    if(!isHaloCellofBoundary(oppositeSide(j), newCellPos)){
                        return;
                    }
                    Cell newCell = container.getCells().at(container.cellIndex(newCellPos[0], newCellPos[1], newCellPos[2]));

                    Particle* existingPartner = nullptr;

                    for (auto p : newCell.getParticlesInCell()){

                        if(p -> getType() != currentParticle.getType()){
                            continue;
                        }

                        if(p -> getID() == currentParticle.getPartner(j)){
                            existingPartner = p;
                            SPDLOG_INFO("found already set partner of {}: {}", currentParticle.getID(), p -> getID());
                            break; //partner was already set correctly
                        }

                        std::array<double,3> location = cloneParticleLocation(currentParticle.getX(), j);
                        std::array<double,3> existingLocation = p->getX();
                        if (abs(location[0] - existingLocation[0]) < 0.00001 &&
                             abs(location[1] - existingLocation[1]) < 0.00001 &&
                             abs(location[2] - existingLocation[2]) < 0.00001){ //is corresponding clone
                            //set partner
                            SPDLOG_INFO("found partner of {}: {}", currentParticle.getID(), p -> getID());
                            p -> setPartner(j, currentParticle.getID());
                            currentParticle.setPartner(j, p->getID());
                            SPDLOG_INFO("set partner: {} has partner {}, {} has partner {} in direction {}", p -> getID(), p -> getPartner(j), currentParticle.getID(), currentParticle.getPartner(j), j);
                            existingPartner = p;
                            break;
                        }
                    }

                    if (existingPartner == nullptr){
                    //create new
                    Particle newParticle = createCloneParticle(j, currentParticle);
                    newParticle.setPartner(j, currentParticle.getID());
                    currentParticle.setPartner(j, newParticle.getID()); //if newParticle moves into domain and particle leaves domain, they switch roles

                     //check if need to create a third/set partner
                        for (int x = 0; x < 6; x++){
                        cloneOfCloneHelper(newParticle, newCellPos, j, x, false);
                        }

                    container.addParticle(newParticle);

        }     

}

double BoundaryHandler::calculateDistance(Particle p, int i) { //passing by value on purpose
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

std::array<double, 3L> BoundaryHandler::ghostParticleLocation(Particle p, int i, double dist){//pass by value
    std::array<double,3> mirrorX = p.getX();

    switch (i) {
        case 0: { //mirror along left border
            mirrorX[0] = (mirrorX[0] - 2 * dist);
            break;
        }
        case 1: { //mirror along right border
            mirrorX[0] = (mirrorX[0] + 2 * dist);   
            break;
        }
        case 2: { //mirror along top border
            mirrorX[1] = (mirrorX[1] + 2 * dist);
            break;
        }
        case 3: { //mirror along bottom border
            mirrorX[1] = (mirrorX[1] - 2 * dist);
            break;
        }
        case 4: { //mirror along front border
            mirrorX[2] = (mirrorX[2] + 2 * dist); //not sure yet if (0|0|0) is front bottom left or back bottom left corner
            break;
        }
        case 5: { //mirror along back border
            mirrorX[2] = (mirrorX[2] - 2 * dist);
            break;
        }
    }

    return mirrorX;
}

bool BoundaryHandler::isHaloCellofBoundary(int i, std::array<int, 3> pos){
    switch (i){
        case 0: return (pos[0] == -1);
        case 1: return (pos[0] == container.getCellNumPerDimension()[0]);
        case 2: return (pos[1] == container.getCellNumPerDimension()[1]);
        case 3: return (pos[1] == -1);
        case 4: if (container.getCellNumPerDimension()[2] == 1){return false;} else return (pos[2] == container.getCellNumPerDimension()[2]);
        case 5: if (container.getCellNumPerDimension()[2] == 1){return false;} else return (pos[2] == -1);
        default: throw std::runtime_error("Error calculating isHaloCellOfBoundary");
    }
    
}

bool BoundaryHandler::isBoundaryCellofBoundary(int i, std::array<int, 3> pos){
     switch (i){
        case 0: return (pos[0] == 0);
        case 1: return (pos[0] == container.getCellNumPerDimension()[0] - 1);
        case 2: return (pos[1] == container.getCellNumPerDimension()[1] - 1);
        case 3: return (pos[1] == 0);
        case 4: if (container.getCellNumPerDimension()[2] == 1){return false;} else return (pos[2] == container.getCellNumPerDimension()[2] - 1);
        case 5: if (container.getCellNumPerDimension()[2] == 1){return false;} else return (pos[2] == 0);
        default: throw std::runtime_error("Error calculating isBoundaryCellOfBoundary");
    }
}

bool BoundaryHandler::movedIntoBoundary(int i, std::array<double, 3> oldX) {
if (isnan(oldX[0]) && isnan(oldX[1]) && isnan(oldX[2])) {
    return true; //initial case
}

    switch (i) {
        case 0: return (static_cast<int>(std::floor(oldX[0] / container.getCellSizePerDimension()[0]))) > 0;
        case 1: return (static_cast<int>(std::floor(oldX[0] / container.getCellSizePerDimension()[0]))) < container.getCellNumPerDimension()[0] - 1;
        case 2: return (static_cast<int>(std::floor(oldX[1] / container.getCellSizePerDimension()[1]))) < container.getCellNumPerDimension()[1] - 1;
        case 3: return (static_cast<int>(std::floor(oldX[1] / container.getCellSizePerDimension()[1]))) > 0;
        case 4: return (static_cast<int>(std::floor(oldX[2] / container.getCellSizePerDimension()[2]))) < container.getCellNumPerDimension()[2] - 1;
        case 5: return (static_cast<int>(std::floor(oldX[2] / container.getCellSizePerDimension()[2]))) > 0;
        default: throw std::runtime_error("Error calculating movedIntoBoundary");
    }

}

Particle BoundaryHandler::createCloneParticle(int i, Particle particle){
    std::array <double, 3> newPosition = particle.getX();
    std::array <double, 3> newFormerPosition = {NAN, NAN, NAN}; //this might be outside the domain, we are currently not using oldX for any calculations this is just in case

    newPosition = cloneParticleLocation(newPosition, i);

    Particle newParticle = Particle({newPosition}, {particle.getV()}, particle.getM(), particle.getType(), particle.getEpsilon(), particle.getSigma());
    newParticle.setF(particle.getF());
    newParticle.setOldF(particle.getOldF());
    newParticle.setOldX(newFormerPosition);
    return newParticle;
}

Particle BoundaryHandler::createCloneParticle(int i, int j, Particle particle){
    std::array <double, 3> newPosition = particle.getX();
    std::array <double, 3> newFormerPosition = particle.getOldX(); //this might be outside the domain, we are currently not using oldX for any calculations this is just in case

    newPosition = cloneParticleLocation(cloneParticleLocation(newPosition, i), j);
    newFormerPosition = cloneParticleLocation(cloneParticleLocation(newFormerPosition, i), j);

    Particle newParticle = Particle({newPosition}, {particle.getV()}, particle.getM(), particle.getType(), particle.getEpsilon(), particle.getSigma());
    newParticle.setF(particle.getF());
    newParticle.setOldF(particle.getOldF());
    newParticle.setOldX(newFormerPosition);
    return newParticle;
}

std::array<double, 3UL> BoundaryHandler::cloneParticleLocation(std::array <double, 3UL> pos, int i){
    switch (i) {
        case 0: {
            pos[0] += container.getDomainSize()[0];
            return pos;
        }
        case 1:
        {
            pos[0] -= container.getDomainSize()[0];
            return pos;
        }
        case 2:
        {
            pos[1] -= container.getDomainSize()[1];
            return pos;
        }
        case 3:
        {
            pos[1] += container.getDomainSize()[1];
            return pos;
        }
        case 4:
        {
            pos[2] -= container.getDomainSize()[2];
            return pos;
        }
        case 5:
        {
            pos[2] += container.getDomainSize()[2];
            return pos;
        }
        default: throw std::runtime_error("Error calculating cloneParticleLocation");
    }
};
   


void BoundaryHandler::handleCornersPeriodic(int i, Cell& cell) {
    for (int j = i + 1; j < 6; j++) { //every pair is looked at once

        if (!(((i == 0 || i == 1) && (j == 2 || j == 3)) || ((i == 0 || i == 1) && (j == 4 || j == 5)) || ((i == 2 || i == 3) && (j == 4 || j == 5)))){
            continue;
        }

        if (type[j] != bCondition::PERIODIC) {
            continue;
        }

        if (!isBoundaryCellofBoundary(j, cell.getPosition())) {
            continue;
        }

        for (auto particle:cell.getParticlesInCell()){
            if (movedIntoBoundary(i, particle->getOldX()) && movedIntoBoundary (j, particle -> getOldX())){
                Particle newParticle = createCloneParticle(i, j, *particle);
                int partnerI = particle->getPartner(i);
                int partnerJ = particle->getPartner(j);
                newParticle.setPartner(j, partnerI);
                newParticle.setPartner(i, partnerJ);

                //find those particles and set their partner
                std::array<int, 3> pos = oppositeCell(cell.getPosition(), i);
                Cell& opposite = container.getCells().at(container.cellIndex(pos[0], pos[1], pos[2]));

                for (auto & p: opposite.getParticlesInCell()){
                    if (p->getID() == partnerI){
                        p->setPartner(j, newParticle.getID());
                        break;
                    }
                }

                pos = oppositeCell(cell.getPosition(), j);
                opposite = container.getCells().at(container.cellIndex(pos[0], pos[1], pos[2]));

                for (auto p: opposite.getParticlesInCell()){
                    if (p->getID() == partnerJ){
                        p->setPartner(i, newParticle.getID());
                        break;
                    }
                }

                container.addParticle(newParticle);
            }
        }
    }

}

double BoundaryHandler::minDist(double sigma){
    return std::pow(2.0, 1.0/6.0) * sigma;
}

void BoundaryHandler::updatePartners(){

    std::vector<int> cornercells; //Halo cell that belong to two boundaries
    std::array <int, 3> cellNum = container.getCellNumPerDimension();
    int numberOfCornerCells = (cellNum[0] + cellNum[1] + cellNum[2]) * 4 + 8;
    cornercells.reserve(numberOfCornerCells);

    std::vector<int> doubleCornercells; //halo cells that belong to three boundaries
    doubleCornercells.reserve(8);

    int index = -1;
    for (auto cell: container.getHaloCells()){ //first do normal halo cells (clones)

        index++;

        int isCorner = isCornerHaloCell(cell.get().getPosition());
        
        if (isCorner == 1){
            cornercells.push_back(index); //save corner cells to do later
        } else if (isCorner == 2){
            doubleCornercells.push_back(index);
        } else { //not corner cell
              updateCell(cell.get());
        }
      
    }

    //now do the corner cells (clones of clones)

    for (auto cellIndex : cornercells){
            
            Cell& cell = container.getHaloCells().at(cellIndex);
            
            updateCell(cell);
    }

    //now do double corner cells (clones of clones of clones)

    for (auto cellIndex : doubleCornercells){
            
            Cell& cell = container.getHaloCells().at(cellIndex);
            
            updateCell(cell);
    }
}

void BoundaryHandler::updateCell(Cell& cell){

    for (int i = 0; i < 6; i++){

            if (type[oppositeSide(i)] != bCondition::PERIODIC){
                continue;
            }

            if(!isHaloCellofBoundary(i, cell.getPosition())){
                continue;
            }

        std::array<int, 3> pos = oppositeCell(cell.getPosition(), i);
        Cell oppositeCell = container.getCells().at(container.cellIndex(pos[0], pos[1], pos[2]));

        for (auto particle: cell.getParticlesInCell()){

            if (particle->getPartner(i) == 0){ //not a cloned particle
                continue;
            }
            
            for (auto p : oppositeCell.getParticlesInCell()){
                if (particle -> getPartner(i) == p ->getID()){
                    particle -> setF(p -> getF());
                    particle -> setOldF(p -> getOldF());
                    particle -> setV(p -> getV());
                    break;
                }
            }
        }
    }

}


int BoundaryHandler::oppositeSide(int i){
    switch (i){
        case 0: return 1;
        case 1: return 0;
        case 2: return 3;
        case 3: return 2;
        case 4: return 5;
        case 5: return 4;
        default: return -1;
    }
}

std::array<int, 3>  BoundaryHandler::oppositeCell(std::array<int,3> position, int i){
    switch(i){
        case 0: position[0] = position[0] + container.getCellNumPerDimension()[0]; break;
        case 1: position[0] = position[0] - container.getCellNumPerDimension()[0]; break;
        case 2: position[1] = position[1] - container.getCellNumPerDimension()[1]; break;
        case 3: position[1] = position[1] + container.getCellNumPerDimension()[1]; break;
        case 4: position[2] = position[2] - container.getCellNumPerDimension()[2]; break;
        case 5: position[2] = position[2] + container.getCellNumPerDimension()[2]; break;
        default: throw std::runtime_error("Error calculating oppositeCell");
    }

    return position;
}

 int BoundaryHandler::isCornerHaloCell (std::array<int, 3> position){
    bool a = isHaloCellofBoundary(0, position);
    bool b = isHaloCellofBoundary(1, position);
    bool c = isHaloCellofBoundary(2, position);
    bool d = isHaloCellofBoundary(3, position);
    bool e = isHaloCellofBoundary(4, position);
    bool f = isHaloCellofBoundary(5, position);

    if ((a || b) && (c || d) && (e || f)){
        return 2;
    }

    if (((a || b) && (c || d))|| ((a || b ) && (e || f)) || ((c || d) && (e || f))){
        return 1;
    } 

    return 0;
 }


