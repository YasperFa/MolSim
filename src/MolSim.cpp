
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
 * helper function for multiplying a vector with constant;
 */
std::array<double, 3>  multiply_constant_vector(const std::array<double, 3> &a,  double b);
/**
 * helper function for calculating magnitude
 */
double magnitude(const std::array<double, 3> &a);
constexpr double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;

// TODO: what data structure to pick?
std::list<Particle> particles;

int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 2) {
    if (argc == 4) {
       delta_t = atof(argsv[2]);
       end_time = atof(argsv[3]);
    }
    else {
      std::cout << "Erroneous programme call! " << std::endl;
      std::cout << "./molsym filename" << std::endl;
    }
  }
  std::string out_name("test");
  FileReader fileReader;
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
    if (iteration % 10 == 0) {
      plotParticles(iteration);
      outputWriter::VTKWriter writer;
      writer.initializeOutput(particles.size());
      for (auto &p : particles) {
        writer.plotParticle(p);
      }
      writer.writeFile(out_name, iteration);
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