/*
 * ProfilePrinter.h
 *
 *  Created on: 22.01.2025
 *      Author: Stefanie Blattenberger
 */

/**Class for printing particle density- and velocity-profile along the x-Axis */

#include <string>
#include <fstream>
#include "Objects/Particle.h"
#include "Objects/Containers/DirectSum/DirectSumContainer.h"
#include "Objects/Containers/LinkedCell/LinkedCellContainer.h"


class ProfilePrinter{
    public:
    ProfilePrinter(ParticleContainers::ParticleContainer& particleContainer);
    ~ProfilePrinter() = default;
    void initializeOutput();
    void printOutput(ParticleContainers::ParticleContainer& particleContainer, int iteration);
    

    private:
    double smallest;
    double biggest;
    double bucketSize;

    std::array<double, 50> velocityBuckets;
    std::array<int, 50> densityBuckets;


    int mapToBucket(double xValue);
    void clearBuckets();
    void updateBuckets(ParticleContainers::DirectSumContainer& particleContainer);

};