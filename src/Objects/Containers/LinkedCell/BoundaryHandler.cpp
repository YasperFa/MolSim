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
//SPDLOG_DEBUG("type set to {} {} {} {}", type[0], type[1], type [2], type [3]);
};


void BoundaryHandler::handleBoundaries(){
            handlePeriodic();
            handleReflecting();
            handleOutflow();
            updatePartners();
    }


void BoundaryHandler::handleOutflow(){
    SPDLOG_TRACE("handle outflow");

    for (int i = 0; i < 4; i++) { //FOR 3D: 6

     if (type[i] != bCondition::OUTFLOW) {continue;}

        for (auto cell : container.getHaloCells()) {

            if (!isHaloCellofBoundary(i, cell.get().getPosition())){continue;}

            for (auto p : cell.get().getParticlesInCell()) {

                if (p->getState() == Particle::State::DEAD){
                    continue;
                }

                container.removeParticle(*p);
                container.updateParticlesInCell();
            }

        }
       
    } 
  
};

void BoundaryHandler::handleReflecting(){

    SPDLOG_TRACE("handle reflecting");

    for (int i = 0; i < 4; i++) {//0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 2 dimensions only for now

        if (type [i] != bCondition::REFLECTING) {continue;}
   
        for (auto cell : container.getBoundaryCells()){

            if (!isBoundaryCellofBoundary(i, cell.get().getPosition())) {continue;}
        
            for (Particle * p : cell.get().getParticlesInCell()) {

                if (p->getState() == Particle::State::DEAD){
                    continue;
                }
  
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

                if (p->getState() == Particle::State::DEAD){
                    continue;
                }
  
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

    for (int i = 0; i < 4; i++) {

        if (type [i] != bCondition::PERIODIC) {continue;}

        for (auto cell : container.getBoundaryCells()){
            if (!isBoundaryCellofBoundary(i, cell.get().getPosition())) {continue;}

            for (auto particle : cell.get().getParticlesInCell()) {

                if (particle->getState() == Particle::State::DEAD){
                    continue;
                }
                
                if (!movedIntoBoundary(i, particle -> getOldX())) {continue;} //only create clone particle if particle moved into boundary in this iteration
                Particle newParticle = createCloneParticle(i, *particle);
                newParticle.setPartner(particle -> getID());
                particle -> setPartner(newParticle.getID()); //if newParticle moves into domain and particle leaves domain, they switch roles
                container.addParticle(newParticle);
                //container.updateParticlesInCell();
            }

        //if is corner cell
        handleCornersPeriodic(i, cell);
        }
        
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
        case 4: return (pos[2] == container.getCellNumPerDimension()[2]);
        case 5: return (pos[2] == -1);
        default: throw std::runtime_error("Error calculating isHaloCellOfBoundary");
    }
    
}

bool BoundaryHandler::isBoundaryCellofBoundary(int i, std::array<int, 3> pos){
     switch (i){
        case 0: return (pos[0] == 0);
        case 1: return (pos[0] == container.getCellNumPerDimension()[0] - 1);
        case 2: return (pos[1] == container.getCellNumPerDimension()[1] - 1);
        case 3: return (pos[1] == 0);
        case 4: return (pos[2] == container.getCellNumPerDimension()[2] - 1);
        case 5: return (pos[2] == 0);
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
    std::array <double, 3> newFormerPosition = particle.getOldX(); //this might be outside the domain, we are currently not using oldX for any calculations this is just in case

    newPosition = cloneParticleLocation(newPosition, i);
    newFormerPosition = cloneParticleLocation(newFormerPosition, i);

    Particle newParticle = Particle({newPosition}, {particle.getV()}, particle.getM(), particle.getType(), particle.getEpsilon(), particle.getSigma());
    newParticle.setF(particle.getF());
    newParticle.setOldF(particle.getOldF());
    newParticle.setOldX(newFormerPosition);
    newParticle.setState(Particle::State::ALIVE);
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
    for (int j = i + 1; j < 4; j++) { //every pair is looked at once

        if (!((i == 0 || i == 1) && (j == 2 || j == 3))){
            continue;
        }

        if (type[j] != bCondition::PERIODIC) {
            continue;
        }

        if (!isBoundaryCellofBoundary(j, cell.getPosition())) {
            continue;
        }

        for (auto particle:cell.getParticlesInCell()){

            if (particle->getState() == Particle::State::DEAD){
                    continue;
                }

            if (movedIntoBoundary(i, particle->getOldX()) && movedIntoBoundary (j, particle -> getOldX())){
                Particle newParticle = createCloneParticle(i, j, *particle);
                newParticle.setPartner(particle -> getID());
                particle->setPartner(newParticle.getID());
                container.addParticle(newParticle);
            }
        }
    }

}

double BoundaryHandler::minDist(double sigma){
    return std::pow(2.0, 1.0/6.0) * sigma;
}

void BoundaryHandler::updatePartners(){

    for (auto cell: container.getHaloCells()){

        for (int i = 0; i < 4; i++){

            if (type[oppositeSide(i)] != bCondition::PERIODIC){
                continue;
            }

            if(!isHaloCellofBoundary(i, cell.get().getPosition())){
                continue;
            }

        std::array<int, 3> pos = oppositeCell(cell.get().getPosition(), i);
        Cell oppositeCell = container.getCells().at(container.cellIndex(pos[0], pos[1], pos[2]));

        for (auto particle: cell.get().getParticlesInCell()){

            if (particle->getState() == Particle::State::DEAD){
                    continue;
                }

            for (auto p : oppositeCell.getParticlesInCell()){

                 if (p->getState() == Particle::State::DEAD){
                    continue;
                }

                if (particle -> getPartner() == p ->getID()){
                    particle -> setF(p -> getF());
                    particle -> setOldF(p -> getOldF());
                    particle -> setV(p -> getV());
                }
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
        default: throw std::runtime_error("Error calculating oppositeSide");
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

