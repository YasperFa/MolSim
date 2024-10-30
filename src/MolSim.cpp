
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>
#include <functional>

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
 * @param int iteration
 */
void plotParticles(int iteration);
/**
 * helper function for adding two vectors
 * @param a first vector
 * @param b second vector
 * @returns vector sum of a and b
 */

std::array<double, 3>  addVector(const std::array<double, 3> &a,const std::array<double, 3> &b);
/**
 * helper function for subtracting two vectors
 * @param a first vector
 * @param b second vector
 * @returns vector difference of a and b
 */
std::array<double, 3>  subtractVector(const std::array<double, 3> &a, const std::array<double, 3> &b);
/**
 * helper function for multiplying a vector with constant
 * @param a first vector
 * @param b double value
 * @returns product of a and b
 */
std::array<double, 3>  multiply_constant_vector(const std::array<double, 3> &a,  double b);
/**
 * helper function for calculating magnitude of a vector
 * @param a vector
 * @returns magnitude of a (double)
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

/** data structure for storing particles and unique pairs of particles */
ParticleContainer particles;

/** Prints help message to stderr */

void printHelp() {

  std::cerr << R"(
      If you want to execute the simulation, the program call has to follow this format:
      ./MolSim .{INPUT_PATH} -d {DELTA_T} -e {END_TIME} {OUTPUT_WRITER}

      Options:
        Compulsory Options:
          '{INPUT_PATH}': path to the input file. For example, './input/eingabe-sonne.txt'.
          '{OUTPUT_WRITER}': specifies which output writer will be used. Either -vtk or -xyz has to be chosen.

        Optional Options:
          '{DELTA_T}': Time step which will be used for the simulation. The argument has to be passed with a positive number
          following the format: '-d {positive number}'. If -d is not specified while executing the program, the default
          value d = 0,014 will be used.

          '{END_TIME}': The end time which will be used for the simulation. The argument has to be passed with a positive
          number following the format: '-e {positive number}'. If -e is not specified while executing the program, the
          default value e = 1000 will be used.

        Example Usage:
          './MolSim -h' or './MolSim --help' to print this help message
          './MolSim ../input/eingabe-sonne.txt -vtk'
          './MolSim ../input/eingabe-sonne.txt -d 0.014 -e 1000 -xyz'
      )" << std::endl;
}


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
 * - vtk output (-vtk)
 * - xyz output (-xyz)
 * 
 * Returns 1 if there is no input file or wrong parameters are used.
 * 
 * 
*/


int main(int argc, char *argsv[]) {
  bool xyz = false;
  bool vtk = false;
  std::cout << "Hello from MolSim for PSE!" << std::endl;

       //if input file is not specified --> error
  if (argc < 2) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "Input filename not specified!" << std::endl;
    printHelp();
    return 1;
  }


  if (std::strcmp(argsv[1], "-h") == 0 || std::strcmp(argsv[1], "--help") == 0 ) {
 
    if (2 >= argc) {
        std::cerr << R"(Welcome to MolSim Helper!)" << std::endl;
        printHelp();
      return 0;
    } else {
      //if -h or --h is not the last parameter --> error
      std::cout << "Erroneous programme call! " << std::endl;
      printHelp();
      return 1;
    }
  }


  else {

    //check if input is valid path
   
    const std::string path = argsv[1];
    
    if (path.length() < 5 || path.compare(path.length()- 4, 4, ".txt") != 0) {
      std::cerr << "Invalid input path! Input path must be first argument and file must be .txt" << std::endl;
      printHelp();
      return 1;
    }


    for (int i = 2; i < argc; i++) {
      std::string arg = argsv[i];
        // check if delta_t flag is set
        if (arg == "-d" ) {
          // check if the current parameter is the last one --> delta_t value isn't specified --> error
          if (i+1 >= argc) {
            std::cout << "Erroneous programme call! " << std::endl;
            std::cout << "delta_t not specified!" << std::endl;
            printHelp();
            return 1;}
          // convert a string to double
          delta_t = atof(argsv[i+1]);
          // skip the next parameter since we already processed it
          i++;
          // check if conversion wasn't successful or if input range isn't valid
          if (delta_t <= 0.0) {
            std::cout << "Erroneous programme call! " << std::endl;
            std::cout << "Invalid delta_t" << std::endl;
            printHelp();
           return 1;}

        }
        // check if end_time flag is set
        else if(arg == "-e" ) {
          // check if the current parameter is the last one --> end_time value isn't specified --> error
          if (i+1 >= argc) {
            std::cout << "Erroneous programme call! " << std::endl;
            std::cout << "end_time not specified!" << std::endl;
            printHelp();
           return 1;}
          // convert a string to double
          end_time = atof(argsv[i+1]);
          // skip the next parameter since we already processed it
          i++;
          // check if conversion wasn't successful or if input range isn't valid
          if (end_time <= 0.0) {
            std::cout << "Erroneous programme call! " << std::endl;
            std::cout << "Invalid end_time!" << std::endl;
            printHelp();
           return 1;}

        }
        // check for a vtk flag
        else if(arg == "-vtk" ) {
          vtk = true;
        }
        //check for a xyz flag
        else if(arg == "-xyz" ) {
          xyz = true;
        }
       // argument isn't specified --> error
        else {
         std::cout << "Erroneous programme call! " << std::endl;
         std::cout << "Undefined Parameter! " << std::endl;
         printHelp();
        return 1;
       }
    }
  }

  //if no output writer is specified --> error
  if (xyz == false && vtk == false) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "At least one output writer has to be specified!" << std::endl;
    printHelp();
    return 1;
  }

  //initialize FileReader instance
  FileReader fileReader;
  //read input file that provides initial information about our particles
  fileReader.readToContainer(particles, argsv[1]);
  particles.initializePairsVector();

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
      if(vtk) {
        // define output file name
        std::string out_name("Vtu_");
        // initialize writer instance
        outputWriter::VTKWriter writer;
        // initialize output file
        writer.initializeOutput(particles.sizeParticles());
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
  std::array<double, 3> sigma = {.0,.0,.0};
  for (auto &p : particles) {
    p.setOldF(p.getF());
    p.setF(sigma);
  }

  for (auto &pair : particles.getParticlePairs()) {
    Particle &p1 = pair.first.get();
    Particle &p2 = pair.second.get();

    std::array<double, 3> sub = subtractVector(p2.getX(), p1.getX());
    double normCubed = pow(magnitude(sub),3);

    //prevent division by 0
    if (normCubed == 0) {
      continue;
    }


    std::array<double, 3> fij = multiply_constant_vector(multiply_constant_vector(sub, p1.getM()*p2.getM()),1/normCubed);

    p1.setF(addVector(p1.getF(),fij));
    p2.setF(subtractVector(p2.getF(),fij));
  }



}

void calculateX() {
  for (auto &p : particles) {
    std::array<double, 3> newX = addVector(p.getX(), addVector( multiply_constant_vector(p.getV(),delta_t) , multiply_constant_vector(p.getOldF(),0.5*pow(delta_t,2)/p.getM())));
    p.setX(newX);
 }
}

void calculateV() {
  for (auto &p : particles) {
    std::array<double, 3> newV = addVector(p.getV(), multiply_constant_vector(addVector(p.getOldF(),p.getF()),delta_t*0.5/p.getM()));
    p.setV(newV);
  }
}

void plotParticles(int iteration) {

  std::string out_name("MD_vtk");

  outputWriter::XYZWriter writer;
  writer.plotParticlesFromContainer(particles, out_name, iteration);

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
  return std::sqrt(out);
}