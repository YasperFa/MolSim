#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h" 

#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>
#include <functional>

#include "Objects/ParticleContainer.h"
#include "outputWriter/VTKWriter.h"

#include "Calculator/Calculator.h"
#include "Calculator/lennardJonesCalculator.h"

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


// TODO: what data structure to pick?

/** data structure for storing particles and unique pairs of particles */
ParticleContainer particles;

/** Prints help message to stderr */

void printHelp() {
    std::cerr << R"(
If you want to execute the simulation, the program call has to follow this format:
      
        './MolSim .{INPUT_PATH} -c {CALCULATOR} -d {DELTA_T} -e {END_TIME} {OUTPUT_WRITER} -l {LOG_LEVEL}'

Example calls: 

        './MolSim -h' or './MolSim --help'
        './MolSim ../input/eingabe-sonne.txt -c default -vtk -l debug'
        './MolSim ../input/eingabe-sonne.txt -c default -d 0.014 -e 1000 -xyz'
        './MolSim ../input/cuboid-example.txt -c LJCalculator -vtk -d 0.0002 -e 5'

    The output should be in the build directory.    

Arguments:

    Compulsory arguments:

        '{INPUT_PATH}': Path to the input file. For example, '../input/eingabe-sonne.txt' or '../input/cuboid-example.txt'.

        '{OUTPUT_WRITER}': Specifies which output writer will be used. Either -vtk or -xyz has to be chosen.

    Optional arguments:

        '{CALCULATOR}': Specifies which calculator will be used druing the program execution. If no calculator is specified
         the default calculator will be used. If a calculator is specified this argument has to come directly after the
         {INPUT-PATH}. The argument has to be passed with a valid calculator type with the following format:
         '-c {calculator type}'
         The implemented calculators right now are 'default' and 'LJCalculator'

        '{DELTA_T}': Time step which will be used for the simulation. The argument has to be passed with a positive number
        following the format: '-d {positive number}'
        If -d is not specified while executing the program, for the default calculator the value d = 0.014 and for the LJCalculator
        the default value d = 0.0002 will be used.

        '{END_TIME}': The end time used for the simulation. The argument has to be passed with a positive number
        following the format: '-e {positive number}'
        If -e is not specified while executing the program, or the default calculator the value e = 1000 and for the LJCalculator
        the default value d = 5 will be used.

         '{LOG_LEVEL}': The log level that is to be used. It is not possible to set a log level higher than the compile-time 
        log level at runtime (see 2.). 
        Possible values are, in ascending order: 'off', 'error', 'warn', 'info', 'debug' and 'trace' / 'all'. 
        If -l is not specified, the log level specified at compile time will be used.
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

    spdlog::set_level(spdlog::level::trace); //real level is defined by macro & manual override

    //set log level first
    std::string arg;
    for (int i = 2; i < argc; i++) {
        arg = argsv[i];
        if (arg == "-l") { //check for optional log level override
                // check if the current parameter is the last one --> error
                if (i + 1 >= argc) {
                    SPDLOG_ERROR("Erroneous programme call! log level not specified");
                    printHelp();
                    return 1;
                }
                // skip the next parameter since we already processed it
                i++;
                if (std::strcmp(argsv[i], "off") == 0) {
                    spdlog::set_level(spdlog::level::off);
                } else if (std::strcmp(argsv[i], "trace") == 0) {
                    spdlog::set_level(spdlog::level::trace);
                 } else if (std::strcmp(argsv[i], "debug") == 0) {
                    spdlog::set_level(spdlog::level::debug);
                 } else if (std::strcmp(argsv[i], "info") == 0) {
                    spdlog::set_level(spdlog::level::info);
                 } else if (std::strcmp(argsv[i], "warn") == 0) {
                    spdlog::set_level(spdlog::level::warn);
                 } else if (std::strcmp(argsv[i], "error") == 0) {
                    spdlog::set_level(spdlog::level::err);
                 } else if (std::strcmp(argsv[i], "all") == 0) {
                    spdlog::set_level(spdlog::level::trace);
                 } else {
                    SPDLOG_ERROR("Erroneous programme call! Invalid log level");
                    printHelp();
                    return 1;
                 }

                 SPDLOG_DEBUG("log level was set to {}", argsv[i]);
                 break;
            } 
        
    }

    bool xyz = false;
    bool vtk = false;
    bool LJCalc = false;
    bool defaultCalc = true;
    SPDLOG_INFO("Hello from MolSim for PSE!");

    //if input file is not specified --> error
    if (argc < 2) {
        SPDLOG_ERROR("Erroneous programme call! Input filename not specified!");
        printHelp();
        return 1;
    }


    if (std::strcmp(argsv[1], "-h") == 0 || std::strcmp(argsv[1], "--help") == 0) {
        if (2 >= argc) {
            std::cerr << R"(Welcome to MolSim Helper!)" << std::endl;
            SPDLOG_DEBUG("Helper was called");
            printHelp();
            return 0;
        } else {
            //if -h or --h is not the last parameter --> error
            SPDLOG_ERROR("Erroneous programme call! Too many arguments");
            printHelp();
            return 1;
        }
    } else {
        //check if input is valid pat

        const std::string path = argsv[1];

        if (path.length() < 5 || path.compare(path.length() - 4, 4, ".txt") != 0) {
            SPDLOG_ERROR("Invalid input path! Input path must be first argument and file must be .txt");
            printHelp();
            return 1;
        }

        int i = 2;

        //check if the calculator that is supposed to be used is specified.
        if (std::string(argsv[i]) == "-c") {
            //if -c is the last argument, fail.
            if (3 >= argc) {
                SPDLOG_ERROR("Erroneous programme call! calculator not specified");
                printHelp();
                return 1;
            }
            //set the calculator to default calculator.
            if (std::string(argsv[i + 1]) == "default") {
                defaultCalc = true;
                i += 2;
                SPDLOG_DEBUG("Default Calculator was chosen");
            } else if (std::string(argsv[i + 1]) == "LJCalculator") {
                //set the calculator to a LJ Calculator and change the default parameters in case they are not specified
                //in the upcoming arguments.
                LJCalc = true;
                defaultCalc = false;
                end_time = 5;
                delta_t = 0.0002;
                i += 2;
                SPDLOG_DEBUG("LJ Calculator was chosen");
                SPDLOG_DEBUG("Default values for e and d changed");
            } else {
                //if an invalid calculator is given, fail.
                SPDLOG_ERROR("Erroneous programme call! invalid calculator");
                printHelp();
                return 1;
            }
        }


        for (; i < argc; i++) {
            arg = argsv[i];
            // check if delta_t flag is set
            if (arg == "-d") {
                // check if the current parameter is the last one --> delta_t value isn't specified --> error
                if (i + 1 >= argc) {
                    SPDLOG_ERROR("Erroneous programme call! delta_t not specified");
                    printHelp();
                    return 1;
                }
                // convert a string to double
                delta_t = atof(argsv[i + 1]);
                // skip the next parameter since we already processed it
                i++;
                // check if conversion wasn't successful or if input range isn't valid
                if (delta_t <= 0.0) {
                    SPDLOG_ERROR("Erroneous programme call! Invalid delta_t");
                    printHelp();
                    return 1;
                }
                SPDLOG_DEBUG("delta_t set to {}", delta_t);
            }
            // check if end_time flag is set
            else if (arg == "-e") {
                // check if the current parameter is the last one --> end_time value isn't specified --> error
                if (i + 1 >= argc) {
                    SPDLOG_ERROR("Erroneous programme call! end_time not specified");
                    printHelp();
                    return 1;
                }
                // convert a string to double
                end_time = atof(argsv[i + 1]);
                // skip the next parameter since we already processed it
                i++;
                // check if conversion wasn't successful or if input range isn't valid
                if (end_time <= 0.0) {
                    SPDLOG_ERROR("Erroneous programme call! Invalid end_time");
                    printHelp();
                    return 1;
                }
                SPDLOG_DEBUG("end_time set to {}", end_time);
            }
            // check for a vtk flag
            else if (arg == "-vtk") {
                vtk = true;
                SPDLOG_DEBUG("Output writer set to vtk");
            }
            //check for a xyz flag
            else if (arg == "-xyz") {
                xyz = true;
                SPDLOG_DEBUG("Output writer set to xyz");
            }
            else if (arg == "-l") { //skip this; already specified
            i++;}
            // argument isn't specified --> error
            else {
                SPDLOG_ERROR("Erroneous programme call! Undefined Parameter");
                printHelp();
                return 1;
            }
        }
    }

    //if no output writer is specified --> error
    if (xyz == false && vtk == false) {
        SPDLOG_ERROR("Erroneous programme call! At least one output writer has to be specified!");
        printHelp();
        return 1;
    }

    //initialize FileReader instance
    FileReader fileReader;
    //read input file that provides initial information about our particles
    fileReader.readFile(particles, argsv[1]);
    SPDLOG_TRACE("File reader initialized and file read");
    particles.initializePairsVector();
    SPDLOG_TRACE("Pairs vector initialized");

    double current_time = start_time;
    int iteration = 0;


    //calculator that will be used for the simulation
    std::unique_ptr<Calculators::Calculator> calculator;
    if (LJCalc == true) {
        calculator = std::make_unique<Calculators::lennardJonesCalculator>();
    } else if (defaultCalc == true) {
        calculator = std::make_unique<Calculators::Calculator>();
    }

    SPDLOG_INFO("Beginning calculation");
    // for this loop, we assume: current x, current f and current v are known
    while (current_time < end_time) {
        calculator->calculateXFV(particles, delta_t);

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

        SPDLOG_TRACE("Iteration {} finished.", iteration);

        current_time += delta_t;
    }
    SPDLOG_INFO("Output written. Terminating...");
    return 0;
}

void plotParticles_XYZ(int iteration) {
    SPDLOG_TRACE("Plotting particles XYZ");
    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticlesFromContainer(particles, out_name, iteration);
}

void plotParticles_VTK(int iteration) {
    SPDLOG_TRACE("Plotting particles VTK");
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
