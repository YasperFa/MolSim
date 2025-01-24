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

    std::vector<std::tuple<long, double, double> > neighbourParticles;

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

    // Connection functions for harmonic force
    /**
     * @brief Adds a connected particle with the given parameters
     * @param ptr_diff Difference in memory addresses between this and the connected particle
     * @param l0 Preferred distance (rest length of the spring)
     * @param k Spring constant
     */
    void addNeighbourParticle(long ptr_diff, double l0, double k);

    /**
     * @return The list of connected particles
     */
    const std::vector<std::tuple<long, double, double> > &getNeighbourParticles() const;

    void markForUpwardForce();

    bool upwardForceMarked() const;

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
