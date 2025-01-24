/*
 * ProfilePrinter.cpp
 *
 *  Created on: 22.01.2025
 *      Author: Stefanie Blattenberger
 */

#include "ProfilePrinter.h"
#include "spdlog/spdlog.h"
#include "utils/ArrayUtils.h"


ProfilePrinter::ProfilePrinter(ParticleContainers::ParticleContainer& container) {
    smallest = INT32_MAX;
    biggest = INT32_MIN;

    if (auto lcCont = dynamic_cast<ParticleContainers::LinkedCellContainer *>(&container)) { //LCC
            smallest = 0;
            biggest = lcCont->getDomainSize()[0];
            bucketSize = (biggest - smallest) / 50.0;
    } else {
        auto dsCont = dynamic_cast<ParticleContainers::DirectSumContainer *>(&container);
        updateBuckets(*dsCont);
    }
}




void ProfilePrinter::updateBuckets(ParticleContainers::DirectSumContainer& container){


    for (auto p : container.getParticles()){
        double xValue = p.getX()[0];
        if(xValue < smallest){
            smallest = xValue;
        }

        if (xValue > biggest){
            biggest = xValue;
        }}

    bucketSize = (biggest - smallest) / 50.0;
}


void ProfilePrinter::clearBuckets(){
    for (int i = 0; i < 50; i++){
        velocityBuckets[i] = 0;
        densityBuckets[i] = 0;
    }
}

int ProfilePrinter::mapToBucket(double xValue){
    int index = (int) ((xValue - smallest) / bucketSize); //round down by casting

    if (index == 50){
        return 49; //particle is biggest
    } else if (index > 50){
        SPDLOG_ERROR("Invalid bucket index! Too big");
        return 49;
    } else if(index < 0){
        SPDLOG_ERROR("Invalid bucket index! Too small index {} for xValue {} bucketSize {}", index, xValue, bucketSize);
        return 0;
    }

    return index;
}

void ProfilePrinter::printOutput(ParticleContainers::ParticleContainer& particleContainer, int iteration){

     if (auto dsCont = dynamic_cast<ParticleContainers::DirectSumContainer *>(&particleContainer)) { //LCC
      updateBuckets(*dsCont);
    } 
    //no update necessary for lcc

    clearBuckets();
    
    for (auto particle : particleContainer.getParticles()){
        if(particle.getFixed()){
            continue;
        }

        int index = mapToBucket(particle.getX()[0]);
        densityBuckets[index] ++;
        velocityBuckets[index] += ArrayUtils::L2Norm(particle.getV());
        
    }

    std::ofstream file;
    file.open ("_particleProfile.csv", std::ios::app);
     if(!file.is_open()){
        SPDLOG_ERROR("Unable to open file for particle profile");
    }
    
    for (int i = 0; i < 50; i++){
        double num = densityBuckets[i];
        double vel = 0;

        //average velocity (density equals number of particles)
        if(num != 0){
            vel = velocityBuckets[i]/num;
        }

        //average density over length
        double den = num/bucketSize;
         
    //print
  
    file << iteration << "," << i << "," << vel << "," << den <<std::endl;

    }

    file.close();

}

void ProfilePrinter::initializeOutput(){
    std::ofstream file;
    file.open ("_particleProfile.csv", std::ios::trunc);

    if(!file.is_open()){
        SPDLOG_ERROR("Unable to open file for particle profile");
    }

    file << "Iteration,Bucket,Velocity,Density" << std::endl;

    file.close();

}