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
#include "../../../Calculator/LennardJonesCalculator.h" //only include in .cpp to avoid circular dependency

Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator();

BoundaryHandler::BoundaryHandler(double s, bool t, ParticleContainers::LinkedCellContainer& container) :
sigma {s}, type {t}, container {container}, minDist {std::pow(2.0, 1.0/6.0) * sigma}, 
boundaries {{0, container.getDomainSize()[0], container.getDomainSize()[1], 0, container.getDomainSize()[2], 0}} {};

void BoundaryHandler::handleBoundaries(){
    if (type == 0) {
        handleOutflow();
    } else handleReflecting();
};

void BoundaryHandler::handleOutflow(){
    //SPDLOG_INFO("handle outflow");
    for (auto c : container.getHaloCells()) { 
        for (auto p : c.get().getParticlesInCell()){
            container.removeParticle(*p);
        }
    }
   //container.deleteHaloParticles();
};

void BoundaryHandler::handleReflecting(){

    //SPDLOG_INFO("handle reflecting");
   
    for (auto cell : container.getBoundaryCells()){
        
        for (Particle * p : cell.get().getParticlesInCell()) {
  
            for (int i = 0; i < 4; i++) {//0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 2 dimensions only for now

                //calculate distance from boundary
                double dist = calculateDistance(*p, i);
                //SPDLOG_INFO("{}", dist);
                if (dist < minDist/2) { //must be closer
                    std::array<double, 3> sub = operator-(ghostParticleLocation(*p, i, dist), p->getX());
                    double norm = ArrayUtils::L2Norm(sub);
                    //SPDLOG_INFO("F {} {} {}", p->getF()[0], p->getF()[1], p->getF()[2]);
                    std::array <double, 3UL> force = calculator.calculateFIJ(sub, 0, 0, norm);
                    p->setF(operator+(p -> getF(), force));
                    //SPDLOG_INFO("F {} {} {}", p->getF()[0], p->getF()[1], p->getF()[2]);
                    //SPDLOG_INFO("X {} {} {}", p->getX()[0], p->getX()[1], p->getX()[2]);
                    //SPDLOG_INFO("Ghost {} {} {} {} {}", ghostParticleLocation(*p, i, dist)[0], ghostParticleLocation(*p, i, dist)[1], ghostParticleLocation(*p, i, dist)[2], i, dist);
                   
            }}
        }
    }
        
     
};

/*void BoundaryHandler::handleReflecting(){
   
 std::list<Cell*> haloNeighbors;

    for (auto cell : container.getBoundaryCells()){
     
        //get corresponding shadow fields
        haloNeighbors.clear();
        getHaloNeighbors(cell, haloNeighbors);//max 2 halo neighbors

        for (Particle * p : cell.get().getParticlesInCell()) {
  
            for (int i = 0; i < 4; i++) {//0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 2 dimensions only for now

                //calculate distance from boundary
                double dist = calculateDistance(*p, i);

                if (dist < minDist/2) { //must be closer
                    
                    //test if shadow particle exists
                    bool hasShadowParticle = false;
                    for (auto neighborCell : haloNeighbors) {
                        for(auto particle : neighborCell->getParticlesInCell()) {
                            if (particle->getID() == -p->getID()) { //shadow particle has negated id
                                hasShadowParticle = true;
                                break;
                            }
                        }
                    }

                    //if not already has shadow particle
                    if (!hasShadowParticle) {
                        //create shadow particle 
                        Particle shadow = createShadowParticle (*p, i, dist); //shadow particle has negated id
                        //add particle to shadow area
                        container.addParticle(shadow);
                    }   
                }
            }
        }
    }
        
        //delete unneeded shadow particles

    for (auto c : container.getHaloCells()) {
        //if dist is greater then delete particle
        for (auto shadowP : c.get().getParticlesInCell()) {
            bool canBeDeleted = false;

            for (int i = 0; i < 4; i++) {
                if (calculateDistance(*shadowP, i) >= minDist/2) {
                    canBeDeleted = true;
                }
            }

            if (canBeDeleted) {
                container.removeParticle(*shadowP);
            }
        }
    }
};*/

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
