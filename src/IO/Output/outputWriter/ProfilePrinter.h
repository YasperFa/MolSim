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

/**Class for printing particle profiles
 * Calculates average velocity and density along the x-axis and writes the results to a .csv file
 */
class ProfilePrinter{
    public:
    /** Constructor for ProfilePrinter
     * @param particleContainer the particleContainer that contains the particles to be printed
     * The particleContainer is not saved, but only used to initialize smallest, biggest and bucktSize attributes
     */
    ProfilePrinter(ParticleContainers::ParticleContainer& particleContainer);
    ~ProfilePrinter() = default;

    /** Function for creating the output file and writing the first row (header) */
    void initializeOutput();

    /** Calulates particle profiles for all particles in the container and appends them to the output file
     * @param particleContainer container that contains the particles to be analysed
     * @param itratiion current iteration
     */
    void printOutput(ParticleContainers::ParticleContainer& particleContainer, int iteration);
    

    private:
    /**Smallest x-value to be included in the calculation
     * For DirectSumContainer: equals smallest x-value that any particle has had during the course of the simulation, updates every time printOutput() is called
     * For LinkedCellContainer: equals smallest x-value that is inside the given domain
     */
    double smallest;

    /**Biggest x-value to be included in the calculation
     * For DirectSumContainer: equals biggest x-value that any particle has had during the course of the simulation, updates every time printOutput() is called
     * For LinkedCellContainer: equals biggest x-value that is inside the given domain
     */
    double biggest;

    /**Size of a bucket (total number of buckets is always 50) */
    double bucketSize;

    /** Vector for storing the sum of the velocities of all particles for every bucket */
    std::array<double, 50> velocityBuckets;

    /** Vector for storing the number of particles for every bucket */
    std::array<int, 50> densityBuckets;

    /** Function to map a particle to its corresponding bucket
     * @param xValue the x-coordinate of the particle
     * @returns the bucket index, possible values: {0, 1, ... , 49}
    */
    int mapToBucket(double xValue);

    /**Reset all buckets to 0 in velocityBuckets and densityBuckets */
    void clearBuckets();

    /** Update smallest, biggest and bucket sizes for DirectSumContainer
     * @param particleContainer a DirectSumContainer
     */
    void updateBuckets(ParticleContainers::DirectSumContainer& particleContainer);

};