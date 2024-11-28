/*#include "BoundaryHandler.h"
#include <math.h>
#include <exception>
#include <iostream>
#include <algorithm>

BoundaryHandler::BoundaryHandler(double s, bool t, LinkedCellContainer& container) : //TODO: add boundaries initialization
sigma {s}, type {t}, container {container}, minDist {std::pow(2.0, 1.0/6.0) * sigma}, boundaries {(1, 2, 3, 4, 5, 6)} {};

void BoundaryHandler::handleBoundaries(){
    if (type == 0) {
        handleOutflow();
    } else handleReflecting();
};

void BoundaryHandler::handleOutflow(){
    for (auto c : container.getHaloCells()) { 
        for (auto p : c.get().getParticlesInCell()){
            container.removeParticle(*p);
        }
    }
};

void BoundaryHandler::handleReflecting(){
   
 std::list<Cell*> haloNeighbors;

    for (auto cell : container.getBoundaryCells()){
     
        //get corresponding shadow fields
        haloNeighbors.clear();
        getHaloNeighbors(cell, haloNeighbors);//max 2 halo neighbors

        for (Particle * p : cell.get().getParticlesInCell()) {
  
            for (int i = 0; i < 4; i++) {//0 -> left, 1 -> right, 2 -> top, 3 -> bottom, 2 dimensions only for now

                //calculate distance from boundary
                double dist = calculateDistance(*p, i);

                if (dist <= minDist/2) {
                    
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
                    if (hasShadowParticle) {
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
                if (calculateDistance(*shadowP, i) > minDist/2) {
                    canBeDeleted = true;
                }
            }

            if (canBeDeleted) {
                container.removeParticle(*shadowP);
                delete(shadowP);
            }
        }
    }
};

double BoundaryHandler::calculateDistance(Particle p, int i) { //passing by value on purpose
     switch (i) {
        case 0: return std::sqrt(std::pow((p.getX()[0] - boundaries[0]), 2)); //left
        case 1: return std::sqrt(std::pow((p.getX()[0] - boundaries[1]), 2)); //right
        case 2: return std::sqrt(std::pow((p.getX()[1] - boundaries[2]), 2)); //top
        case 3: return std::sqrt(std::pow((p.getX()[1] - boundaries[3]), 2)); //bottom
        case 4: return std::sqrt(std::pow((p.getX()[2] - boundaries[4]), 2)); //front
        case 5: return std::sqrt(std::pow((p.getX()[2] - boundaries[5]), 2)); //back
        default: throw std::runtime_error("Error calculating distance");
     }
}

void BoundaryHandler::getHaloNeighbors(Cell& c, std::list<Cell*> list){
    for (auto neighbor : c.getNeighbourCells()) {
        if (neighbor->getCellType() == Cell::CType::HALO) {
            list.push_back(neighbor);
        }
    }
}

Particle BoundaryHandler::createShadowParticle(Particle p, int i, double dist){//pass by value
    std::array<double,3> mirrorX = p.getX();
    std::array<double,3> mirrorV = p.getV();
    std::array<double,3> mirrorF = p.getF();
    std::array<double,3> mirrorOldF = p.getOldF();

    switch (i) {
        case 1: { //mirror along left border
            mirrorX[0] = (mirrorX[0] - 2 * dist);
            mirrorV[0] = -mirrorV[0];
            mirrorF[0] = -mirrorF[0];
            mirrorOldF[0] = -mirrorOldF[0];
            break;
        }
        case 2: { //mirror along right border
            mirrorX[0] = (mirrorX[0] + 2 * dist);
            mirrorV[0] = -mirrorV[0];
            mirrorF[0] = -mirrorF[0];
            mirrorOldF[0] = -mirrorOldF[0];
            break;
        }
        case 3: { //mirror along top border
            mirrorX[1] = (mirrorX[1] + 2 * dist);
            mirrorV[1] = -mirrorV[1];
            mirrorF[1] = -mirrorF[1];
            mirrorOldF[1] = -mirrorOldF[1];
            break;
        }
        case 4: { //mirror along bottom border
            mirrorX[1] = (mirrorX[1] - 2 * dist);
            mirrorV[1] = -mirrorV[1];
            mirrorF[1] = -mirrorF[1];
            mirrorOldF[1] = -mirrorOldF[1];
            break;
        }
        case 5: { //mirror along front border
            mirrorX[2] = (mirrorX[2] + 2 * dist); //not sure yet if (0|0|0) is front bottom left or back bottom left corner
            mirrorV[2] = -mirrorV[2];
            mirrorF[2] = -mirrorF[2];
            mirrorOldF[2] = -mirrorOldF[2];
            break;
        }
        case 6: { //mirror along back border
            mirrorX[2] = (mirrorX[2] - 2 * dist);
            mirrorV[2] = -mirrorV[2];
            mirrorF[2] = -mirrorF[2];
            mirrorOldF[2] = -mirrorOldF[2];
            break;
        }
    }

    Particle newParticle = Particle (mirrorX, mirrorV, p.getM(), p.getType());
    newParticle.setF(mirrorF);
    newParticle.setOldF(mirrorOldF);
    newParticle.makeShadowParticle();
    return newParticle;
}
*/