/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "Particle.h"

#include <list>

#include "ParticleContainer.h"

class FileReader {

public:
  FileReader();
  virtual ~FileReader();

  void readFile(std::list<Particle> &particles, char *filename);

  
/**Initializes a ParticleContainer with input data from a file 
 * @param particles: the empty particle container
 * @param filename: path to input file
*/


  void readToContainer(ParticleContainer &particles, char *filename);
};
