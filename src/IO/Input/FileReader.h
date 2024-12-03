/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "Objects/Particle.h"

#include <list>

#include "../../Objects/Containers/DirectSum/DirectSumContainer.h"

class FileReader {

public:
  FileReader();
  virtual ~FileReader();

  static void readFile(ParticleContainers::ParticleContainer &particles, const std::string& filename);

  
/**Initializes a ParticleContainer with input Particles data from a file
 * @param particles: the empty particle container
 * @param filename: path to input file
*/
  static void readParticles(ParticleContainers::ParticleContainer &particles, std::ifstream &input_file);
 /**Initializes a ParticleContainer with input Cuboids data from a file
 * @param particles: the empty particle container
 * @param input_file: stream of file data
 */
 static void readCuboids(ParticleContainers::ParticleContainer &particles, std::ifstream &input_file);
 /**Initializes a ParticleContainer with input Discs data from a file
  * @param particles: the empty particle container
  * @param input_file: stream of file data
  */
 static void readDiscs(ParticleContainers::ParticleContainer &particles, std::ifstream &input_file);
};
