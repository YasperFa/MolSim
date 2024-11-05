#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>
#include <functional>

#include "Objects/ParticleContainer.h"
#include "outputWriter/VTKWriter.h"

#include "Calculator/Calculator.h"

/**** forward declaration of the calculation functions ****/


/**
 * plot the particles to a xyz-file
 * @param int iteration
 */
void plotParticles_XYZ(int iteration);

/**
 * plot the particles to a vtk-file
 * @param int iteration
 */
void plotParticles_VTK(int iteration);


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

/**
 * calculator that will be used for the simulation */
Calculators::Calculator calculator;

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


    if (std::strcmp(argsv[1], "-h") == 0 || std::strcmp(argsv[1], "--help") == 0) {
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
    } else {
        //check if input is valid path

        const std::string path = argsv[1];

        if (path.length() < 5 || path.compare(path.length() - 4, 4, ".txt") != 0) {
            std::cerr << "Invalid input path! Input path must be first argument and file must be .txt" << std::endl;
            printHelp();
            return 1;
        }


        for (int i = 2; i < argc; i++) {
            std::string arg = argsv[i];
            // check if delta_t flag is set
            if (arg == "-d") {
                // check if the current parameter is the last one --> delta_t value isn't specified --> error
                if (i + 1 >= argc) {
                    std::cout << "Erroneous programme call! " << std::endl;
                    std::cout << "delta_t not specified!" << std::endl;
                    printHelp();
                    return 1;
                }
                // convert a string to double
                delta_t = atof(argsv[i + 1]);
                // skip the next parameter since we already processed it
                i++;
                // check if conversion wasn't successful or if input range isn't valid
                if (delta_t <= 0.0) {
                    std::cout << "Erroneous programme call! " << std::endl;
                    std::cout << "Invalid delta_t" << std::endl;
                    printHelp();
                    return 1;
                }
            }
            // check if end_time flag is set
            else if (arg == "-e") {
                // check if the current parameter is the last one --> end_time value isn't specified --> error
                if (i + 1 >= argc) {
                    std::cout << "Erroneous programme call! " << std::endl;
                    std::cout << "end_time not specified!" << std::endl;
                    printHelp();
                    return 1;
                }
                // convert a string to double
                end_time = atof(argsv[i + 1]);
                // skip the next parameter since we already processed it
                i++;
                // check if conversion wasn't successful or if input range isn't valid
                if (end_time <= 0.0) {
                    std::cout << "Erroneous programme call! " << std::endl;
                    std::cout << "Invalid end_time!" << std::endl;
                    printHelp();
                    return 1;
                }
            }
            // check for a vtk flag
            else if (arg == "-vtk") {
                vtk = true;
            }
            //check for a xyz flag
            else if (arg == "-xyz") {
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
    fileReader.readFile(particles, argsv[1]);
    particles.initializePairsVector();

    double current_time = start_time;

    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < end_time) {
        calculator.calculateXFV(particles, delta_t);

        iteration++;
        // produce output files every ten iterations
        if (iteration % 10 == 0) {
            //if xyz flag is set, produce .xyz file
            if (xyz) {
                plotParticles_XYZ(iteration);
            }
            // if vtu flag is set, produce .vtu file
            if (vtk) {
                plotParticles_VTK(iteration);
            }
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void plotParticles_XYZ(int iteration) {
    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticlesFromContainer(particles, out_name, iteration);
}

void plotParticles_VTK(int iteration) {
    // define output file name
    std::string out_name("Vtu_");
    // initialize writer instance
    outputWriter::VTKWriter writer;
    // initialize output file
    writer.initializeOutput(particles.sizeParticles());
    // iterate over all particles and plot them
    for (auto &p: particles) {
        writer.plotParticle(p);
    }
    // produce the output file
    writer.writeFile(out_name, iteration);
}
