/*
* Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>
#include <vector>
#include "ParticleIdInitializer.h"
#include <mutex>

class Particle {
private:
    mutable std::mutex forceMutex;
    /**
     * Position of the particle
     */
    std::array<double, 3> x;
    
    /**
     * Velocity of the particle
     */
    std::array<double, 3> v;

    /**
     * Force effective on this particle
     */
    std::array<double, 3> f;

    /**
     * Force which was effective on this particle
     */
    std::array<double, 3> old_f;

    /**
     * Mass of this particle
     */
    double m;

    /**
     * Type of the particle. Use it for whatever you want (e.g. to separate
     * molecules belonging to different bodies, matters, and so on)
     */
    int type;

    /** Id of the particle. New particles are created with ascending id numbers starting with 1.
     *  Every id is unique.
    */

    int id;

    /**Lennard-Jones parameter epsilon */
    double epsilon;

    /**Lennard-Jones parameter sigma */
    double sigma;

    bool isFixed;

    /**
     * @brief Ids of the direct neighbours of the current particle (up, down, right or left).
     */
    std::vector<int> directNeighbourIds;

    /**
     * @brief Ids of the diagonal neighbours of the current particle.
     */
    std::vector<int> diagonalNeighbourIds;

    /**
     * @brief true if the upward force is supposed to be used on the particle
     */
    bool markedForUpwardForce;

public:
    explicit Particle(int type = 0);

    Particle(const Particle &other);

    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
        int type = 0, double epsilon_arg = 5, double sigma_arg = 1, bool isFixed = false);

    virtual ~Particle();

    const std::array<double, 3> &getX() const;

    const std::array<double, 3> &getV() const;

    const std::array<double, 3> &getF() const;

    const std::array<double, 3> &getOldF() const;

    double getEpsilon();

    double getSigma();

    int getID() const;

    double getM() const;

    int getType() const;

    bool getFixed() const;

    void setF(const std::array<double, 3> &newF);

    void setOldF(const std::array<double, 3> &newOldF);

    void setX(const std::array<double, 3> &X);

    void setV(const std::array<double, 3> &V);

    void fixParticle();

    bool operator==(Particle &other);

    bool operator==(const Particle &other);

    Particle &operator=(const Particle &other);

    //Neighbour functions

    /**
     *
     * @return a vector of direct neighbours of the current particle.
     */
    const std::vector<int> &getDirectNeighbourIds() const;

    /**
     *
     * @return a vector of diagonal neighbours of the current particle.
     */
    const std::vector<int> &getDiagonalNeighbourIds() const;

    /**
     * @brief adds the given particle id to the vector of direct neighbours
     * @param neighbourID the id of the particle to be added
     */
    void addDirectNeighbour(int neighbourID);

    /**
     * @brief adds the given particle id to the vector of diagonal neighbours
     * @param neighbourID the id of the particle to be added
     */
    void addDiagonalNeighbour(int neighbourID);

    /**
     * @brief checks if the given particle is a direct neighbour of the current particle
     * @param other particle to be checked
     * @return true if the particles are direct neighbours vector, else false
     */
    bool isDirectNeighbour(const Particle &other) const;

    /**
    * @brief checks if the given particle is a diagonal neighbour of the current particle
    * @param other particle to be checked
    * @return true if the particles are diagonal neighbours vector, else false
    */
    bool isDiagonalNeighbour(const Particle &other) const;

    /**
    * @brief checks if the given particle is a neighbour of the current particle
    * @param other particle to be checked
    * @return true if the particles are neighbours, else false
    */
    bool isNeighbour(const Particle &other) const;

    void markForUpwardForce();

    bool upwardForceMarked() const;

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
