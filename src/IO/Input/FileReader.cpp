/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include "spdlog/spdlog.h"
#include "Objects/Shapes/Cuboid.h"
#include "Objects/Shapes/Disc.h"
#include "Objects/ParticleGenerator.h"
FileReader::FileReader() = default;

FileReader::~FileReader() = default;

void FileReader::readFile(ParticleContainers::ParticleContainer &particles, const std::string& filename) {
    SPDLOG_DEBUG("reading file {}", filename);

    std::ifstream input_file(filename);

    if (input_file.is_open()) {
        std::string tmp_string;
        getline(input_file, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);
        while (tmp_string.empty() or tmp_string[0] == '#') {
            getline(input_file, tmp_string);
            SPDLOG_DEBUG("Read line: {}", tmp_string);
        }
        std::istringstream numstream(tmp_string);
        int objectType = 0;
        numstream >> objectType;
        SPDLOG_DEBUG("Reading {}", objectType);
        // based on object type call respective function
        switch(objectType) {
            case 0:
                readParticles(particles, input_file);
                break;
            case 1:
                readCuboids(particles, input_file);
                break;
            case 2:
                readDiscs(particles, input_file);
                break;
            default:
                SPDLOG_ERROR("Invalid object in input file");
                throw std::invalid_argument("Invalid object.");
        }
    }
}


void FileReader::readParticles(ParticleContainers::ParticleContainer &particles, std::ifstream &input_file) {
    SPDLOG_DEBUG("reading particles from file");
    std::array<double, 3> x;
    std::array<double, 3> v;
    double m;
    int t; 
    double epsilon;
    double sigma;
    int num_particles = 0;
    std::string tmp_string;
    getline(input_file, tmp_string);
    SPDLOG_DEBUG("Read line: {}", tmp_string);
    std::istringstream numstream(tmp_string);
    numstream >> num_particles;
    SPDLOG_DEBUG("Reading {}.", num_particles);
    getline(input_file, tmp_string);
    SPDLOG_DEBUG("Read line: {}", tmp_string);

    for (int i = 0; i < num_particles; i++) {
        std::istringstream datastream(tmp_string);

        for (auto &xj: x) {
            datastream >> xj;
        }
        for (auto &vj: v) {
            datastream >> vj;
        }
        datastream >> m;
        if (datastream.fail()) {
            SPDLOG_ERROR("Error reading file: unexpected data format");
            exit(-1);
        }

        datastream >> t;
        datastream >> epsilon;
        datastream >> sigma;
        particles.addParticle(Particle(x, v, m, t, epsilon, sigma));

        getline(input_file, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);
    }
}

void FileReader::readCuboids(ParticleContainers::ParticleContainer &particles, std::ifstream &input_file) {
    SPDLOG_DEBUG("reading cuboids from file");
    // define all Cuboid parameters
    std::array<double, 3> x;
    std::array<double, 3> N;
    std::array<double, 3> v;
    double h;
    double m;
    double mv;
    int t;
    double epsilon;
    double sigma;
    // initialize the number of cuboids which will be later read from the file
    int num_cuboids;
    // initialize a string which will contain a line
    std::string tmp_string;
    // get the first line of data from the file. this line will specify the number of cuboids
    getline(input_file, tmp_string);
    SPDLOG_DEBUG("Read line: {}", tmp_string);
    // convert the string to a stream, where strings are seperated by space
    std::istringstream numstream(tmp_string);
    // save the number of cuboids
    numstream >> num_cuboids;
    SPDLOG_DEBUG("Reading {}.", num_cuboids);
    // get the second line of data, which has the parameters of the first cuboid
    getline(input_file, tmp_string);
    SPDLOG_DEBUG("Read line: {}", tmp_string);

    for (int i = 0; i < num_cuboids; i++) {
        std::istringstream datastream(tmp_string);
        // get lower Left front side corner
        for (auto &xj: x) {
            datastream >> xj;
        }
        // get the number of particles per dimension
        for (auto &Nj: N) {
            datastream >> Nj;
        }
        // get the initial velocity
        for (auto &vj: v) {
            datastream >> vj;
        }
        // get the distance between particles
        datastream >> h;
        // get the mass
        datastream >> m;
        // get the mean velocity
        datastream >> mv;
        if (datastream.fail()) {
            SPDLOG_ERROR("Error reading file: unexpected data format");
            exit(-1);
        }

        datastream >> t;
        datastream >> epsilon;
        datastream >> sigma;
        // create a cuboid with the parameters
        Cuboid cuboid(x, N, h, m, v, mv);
        SPDLOG_DEBUG("Cuboid created!: ");
        // generates the particles in the cuboid
        ParticleGenerator::generateCuboid(particles,cuboid, t, epsilon, sigma, -1);
        // read another line (if more cuboids follow)
        getline(input_file, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);
    }
}
void FileReader::readDiscs(ParticleContainers::ParticleContainer &particles, std::ifstream &input_file) {
    SPDLOG_DEBUG("reading discs from file");
    // define all Cuboid parameters
    std::array<double, 3> centerCoord;
    std::array<double, 3> initVel;
    int radius;
    double h;
    double m;
    int t;
    double epsilon;
    double sigma;
    // initialize the number of discs which will be later read from the file
    int numDiscs;
    // initialize a string which will contain a line
    std::string tmp_string;
    // get the first line of data from the file. this line will specify the number of discs
    getline(input_file, tmp_string);
    SPDLOG_DEBUG("Read line: {}", tmp_string);
    // convert the string to a stream, where strings are seperated by space
    std::istringstream numstream(tmp_string);
    // save the number of cuboids
    numstream >> numDiscs;
    SPDLOG_DEBUG("Reading {}.", numDiscs);
    // get the second line of data, which has the parameters of the first disc
    getline(input_file, tmp_string);
    SPDLOG_DEBUG("Read line: {}", tmp_string);

    for (int i = 0; i < numDiscs; i++) {
        std::istringstream datastream(tmp_string);
        // get the center of coordinates
        for (auto &xj: centerCoord) {
            datastream >> xj;
        }
        // get the initial velocity
        for (auto &vj: initVel) {
            datastream >> vj;
        }
        // get the radius
        datastream >> radius;
        // get the distance between particles
        datastream >> h;
        // get the mass
        datastream >> m;
        if (datastream.fail()) {
            SPDLOG_ERROR("Error reading file: unexpected data format");
            exit(-1);
        }

        datastream >> t;
        datastream >> epsilon;
        datastream >> sigma;
        // create a disc with the parameters
       Disc disc(centerCoord, initVel, radius, h, m);
        SPDLOG_DEBUG("Disc created!: ");
        // generates the particles in the disc
        ParticleGenerator::generateDisc(particles,disc, t, epsilon, sigma);
        // read another line (if more discs follow)
        getline(input_file, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);
    }
}
