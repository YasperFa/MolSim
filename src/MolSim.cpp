
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>

#include "outputWriter/VTKWriter.h"

/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX();

/**
 * calculate the position for all particles
 */
void calculateV();

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);
/**
 * helper function for adding two vectors
 */

std::array<double, 3>  addVector(const std::array<double, 3> &a,const std::array<double, 3> &b);
/**
 * helper function for subtracting two vectors
 */
std::array<double, 3>  subtractVector(const std::array<double, 3> &a, const std::array<double, 3> &b);
/**
 * helper function for multiplying a vector with constant
 */
std::array<double, 3>  multiply_constant_vector(const std::array<double, 3> &a,  double b);
/**
 * helper function for calculating magnitude
 */
double magnitude(const std::array<double, 3> &a);
/**
 * time of simulation start
 */
constexpr double start_time = 0;
/**
 * time of simulation end
 */
double end_time = 1000;
/**
 * time between iterations of the simulation */
double delta_t = 0.014;

// TODO: what data structure to pick?

/** data structure for storing particles */

std::list<Particle> particles;

/** \brief Reads programme input and writes output file
 * 
 * @param int argc 
 * @param char* argsv[]
 * 
 * Takes an input file path and optional parameters and flags to do particle simulation and write the output to a .vtu or .xyz file.
 * 
 * Optional parameters/flags: 
 * - delta_t value (-d <double value>)
 * - end_time (-e <double value>)
 * - vtu output (-vtu)
 * - xyz output (-xyz)
 * 
 * Returns 1 if there is no input file or wrong parameters are used.
 * 
 * 
*/

int main(int argc, char *argsv[]) {
  bool xyz = false;
  bool vtu = false;
  std::cout << "Hello from MolSim for PSE!" << std::endl;

  //if input file is not specified --> error
  if (argc < 2) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "input filename not specified" << std::endl;
    return 1;
  }
  else {
    for (int i = 2; i < argc; i++) {
      std::string arg = argsv[i];
        // check if delta_t flag is set
        if (arg == "-d" ) {
          // check if the current parameter is the last one --> delta_t value isn't specified --> error
          if (i+1 >= argc) {
            std::cout << "Erroneous programme call! " << std::endl;
            std::cout << "delta_t not specified" << std::endl;
            return 1;}
          // convert a string to double
          delta_t = atof(argsv[i+1]);
          // skip the next parameter since we already processed it
          i++;
          // check if conversion wasn't successful or if input range isn't valid
          if (delta_t <= 0.0) {
            std::cout << "Erroneous programme call! " << std::endl;
            std::cout << "invalid delta_t" << std::endl;
           return 1;}

        }
        // check if end_time flag is set
        else if(arg == "-e" ) {
          // check if the current parameter is the last one --> end_time value isn't specified --> error
          if (i+1 >= argc) {
            std::cout << "Erroneous programme call! " << std::endl;
            std::cout << "end_time not specified" << std::endl;
           return 1;}
          // convert a string to double
          end_time = atof(argsv[i+1]);
          // skip the next parameter since we already processed it
          i++;
          // check if conversion wasn't successful or if input range isn't valid
          if (end_time <= 0.0) {
            std::cout << "Erroneous programme call! " << std::endl;
            std::cout << "invalid end_time" << std::endl;
           return 1;}

        }
        // check for a vtu flag
        else if(arg == "-vtu" ) {
          vtu = true;
        }
        //check for a xyz flag
        else if(arg == "-xyz" ) {
          xyz = true;
        }
       // argument isn't specified --> error
       else {
         std::cout << "Erroneous programme call! " << std::endl;
         std::cout << "Undefined Parameter! " << std::endl;
        return 1;
       }
    }
  }
  //initialize FileReader instance
  FileReader fileReader;
  //read input file that provides initial information about our particles
  fileReader.readFile(particles, argsv[1]);

  double current_time = start_time;

  int iteration = 0;

  // for this loop, we assume: current x, current f and current v are known
  while (current_time < end_time) {
    // calculate new x
    calculateX();
    // calculate new f
    calculateF();
    // calculate new v
    calculateV();

    iteration++;
    // produce output files every ten iterations
    if (iteration % 10 == 0) {
      //if xyz flag is set, produce .xyz file
      if (xyz) {
        plotParticles(iteration);
      }
      // if vtu flag is set, produce .vtu file
      if(vtu) {
        // define output file name
        std::string out_name("Vtu_");
        // initialize writer instance
        outputWriter::VTKWriter writer;
        // initialize output file
        writer.initializeOutput(particles.size());
        // iterate over all particles and plot them
        for (auto &p : particles) {
          writer.plotParticle(p);
        }
        // produce the output file
        writer.writeFile(out_name, iteration);
      }

    }
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += delta_t;
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}

void calculateF() {
  std::list<Particle>::iterator iterator;
  iterator = particles.begin();
  for (auto &p1 : particles) {
    std::array<double, 3> sigma = {.0,.0,.0};
    for (auto &p2 : particles) {

      if (!p1.operator==(p2)) {
        std::array<double, 3> sub = subtractVector(p1.getX(), p2.getX());
        std::array<double, 3> fij = multiply_constant_vector(multiply_constant_vector(sub,p1.getM()*p2.getM()),1/pow(magnitude(sub),3));
        sigma = addVector(sigma,fij);
      }
    }

  p1.setOldF(p1.getF());
  p1.setF(sigma);

  }
}

void calculateX() {
  for (auto &p : particles) {
    std::array<double, 3> newX = addVector(p.getX(), addVector( delta_t*p.getV() , multiply_constant_vector(multiply_constant_vector(p.getOldF(),pow(delta_t,2)), 0.5/p.getM())));
    p.setX(newX);
 }
}

void calculateV() {
  for (auto &p : particles) {
    std::array<double, 3> newV = addVector(p.getV(), multiply_constant_vector(multiply_constant_vector(addVector(p.getOldF(),p.getF()),0.5/p.getM()),delta_t));
    p.setV(newV);
  }
}

void plotParticles(int iteration) {

  std::string out_name("MD_vtk");

  outputWriter::XYZWriter writer;
  writer.plotParticles(particles, out_name, iteration);

}
std::array<double, 3>  addVector(const std::array<double, 3> &a,const std::array<double, 3> &b) {
  std::array<double, 3> out{};
  for (int i = 0; i < 3; ++i) {
    out[i] = a[i] + b[i];
  }  return out;
}

std::array<double, 3>  subtractVector(const std::array<double, 3> &a,const std::array<double, 3> &b) {
  std::array<double, 3> out{};
  for (int i = 0; i < 3; ++i) {
      out[i] = a[i] - b[i];
  }
  return out;
}
std::array<double, 3>  multiply_constant_vector(const std::array<double, 3> &a, const double b) {
  std::array<double, 3> out{};
  for (int i = 0; i < 3; ++i) {
    out[i] = a[i]*b;
  }
  return out;
}
double magnitude(const std::array<double, 3> &a) {
  double out = 0;
  for (int i = 0; i < 3; ++i) {
    out += pow(a[i], 2);
  }
  return pow(out, 0.5);
}