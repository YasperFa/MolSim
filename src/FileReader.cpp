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
#include "Objects/Cuboid.h"

FileReader::FileReader() = default;

FileReader::~FileReader() = default;

void FileReader::readFile(ParticleContainer &particles, const std::string& filename) {
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
        int object_type = 0;
        numstream >> object_type;
        SPDLOG_DEBUG("Reading {}", object_type);
        if (object_type == 0) {
            readParticles(particles, input_file);
        } else if (object_type == 1) {
            readCuboids(particles, input_file);
        } else {
            SPDLOG_ERROR("Invalid object in input file");
            throw std::invalid_argument("Invalid object.");
        }
    }
}


void FileReader::readParticles(ParticleContainer &particles, std::ifstream &input_file) {
    SPDLOG_DEBUG("reading particles from file");
    std::array<double, 3> x;
    std::array<double, 3> v;
    double m;
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
        if (datastream.eof()) {
            SPDLOG_ERROR("Error reading file: eof reached unexpectedly reading from line {}", i);
            exit(-1);
        }
        datastream >> m;
        particles.addParticle(Particle(x, v, m));

        getline(input_file, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);
    }
}

void FileReader::readCuboids(ParticleContainer &particles, std::ifstream &input_file) {
    SPDLOG_DEBUG("reading cuboids from file");
    // define all Cuboid parameters
    std::array<double, 3> x;
    std::array<double, 3> N;
    std::array<double, 3> v;
    double h;
    double m;
    double mv;
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
        // check if end of line is reach --> not enough data
        if (datastream.eof()) {
            SPDLOG_ERROR("Error reading file: eof reached unexpectedly reading from line {}", i);
            exit(-1);
        }
        // get the mean velocity
        datastream >> mv;
        // create a cuboid with the parameters
        Cuboid cuboid(x, N, h, m, v, mv);
        SPDLOG_DEBUG("Cuboid created!: ");
        // generates the particles in the cuboid
        cuboid.generate_particles(particles);
        // read another line (if more cuboids follow)
        getline(input_file, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);
    }
}
