/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "Objects/Particle.h"

#include <list>

#include "../../Objects/Containers/ParticleContainer.h"

class FileReader {

public:
  FileReader();
  virtual ~FileReader();

  static void readFile(ParticleContainer &particles, const std::string& filename);

  
/**Initializes a ParticleContainer with input Particles data from a file
 * @param particles: the empty particle container
 * @param filename: path to input file
*/
  static void readParticles(ParticleContainer &particles, std::ifstream &input_file);
 /**Initializes a ParticleContainer with input Cuboids data from a file
 * @param particles: the empty particle container
 * @param filename: path to input file
 */
 static void readCuboids(ParticleContainer &particles, std::ifstream &input_file);
};
