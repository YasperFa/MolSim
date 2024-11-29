//
// Created by Stefanie Blattenberger on 29.11.2024
// Adapted from MolSim.h on 29.11.24.
//

#include "MolSimFunctions.h"

void MolSim::printHelp() {
    std::cout << R"(
Welcome to MolSim helper!
If you want to execute the simulation, the program call has to follow this format:

    './MolSim .{INPUT_PATH} -c {CALCULATOR} -d {DELTA_T} -e {END_TIME} {OUTPUT_WRITER} -l {LOG_LEVEL}'
or

    './MolSim --input=.{INPUT_PATH} --calculator={CALCULATOR} --deltaT={DELTA_T} -endTime={END_TIME} --output={OUTPUT_WRITER} --logLevel {LOG_LEVEL}'


Example calls:

    './MolSim -h' or './MolSim --help'
    './MolSim -i ../input/eingabe-sonne.txt -c Default -o VTK -l debug'
    './MolSim --input=../input/eingabe-sonne.txt --calculator=Default --deltaT=0.014 --endTime=1000 --output=XYZ --logLevel=info'
    './MolSim -i ../input/cuboid-example.txt -c LJC -o VTK -d 0.0002 -e 5'

The output should be in the build directory.

Arguments:

Compulsory arguments:

    '{INPUT_PATH}': Path to the input file. For example, '-i ../input/eingabe-sonne.txt' or 'input=../input/cuboid-example.txt'.

    '{OUTPUT_WRITER}': Specifies which output writer will be used. Either VTK or XYZ has to be chosen. Examples: "-o VTK" or "-output XYZ"

Optional arguments:

    '{CALCULATOR}': Specifies which calculator will be used druing the program execution. If no calculator is specified
        the default calculator will be used. The argument has to be passed with a valid calculator type with the following format:
        '-c {calculator type}' or '--calculator={calculator type}'
        The implemented calculators right now are 'Default' and 'LJC'

    '{DELTA_T}': Time step which will be used for the simulation. The argument has to be passed with a positive number
    following the format: '-d {positive number}' or '--deltaT {positive number}'
    If -d is not specified while executing the program, the value d = 0.014 will be used.

    '{END_TIME}': The end time used for the simulation. The argument has to be passed with a positive number
    following the format: '-e {positive number}' or '--endTime {positive number}'
    If -e is not specified while executing the program, the value e = 1000 will be used.

    '{LOG_LEVEL}': The log level that is to be used. It is not possible to set a log level higher than the compile-time
    log level at runtime (see 2.).
    Possible values are, in ascending order: 'off', 'error', 'warn', 'info', 'debug' and 'trace' / 'all'.
    If -l is not specified, the log level specified at compile time will be used.
    The argument has to be passed with the following format: '-l {logLevel}' or '--logLevel {logLevel}'
    )" << std::endl;
}


bool MolSim::parseArguments(int argc, char *argv[], std::string &inputFile, double &deltaT, double &endTime,
                            std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                            std::unique_ptr<Calculators::Calculator> &calculator,
                            std::unique_ptr<ParticleContainers::ParticleContainer> &particleContainer) {
    cxxopts::Options options("MolSim", "Molecular Simulation Of Group G WS24");

    options.add_options()
            ("h,help", "print help message")
            ("i,input", "input file path", cxxopts::value<std::string>())
            ("d,deltaT", "Set deltaT", cxxopts::value<double>()->default_value("0.014"))
            ("e,endTime", "Set endTime", cxxopts::value<double>()->default_value("1000"))
            ("o,output", "Set Outputwriter (VTK or XYZ)", cxxopts::value<std::string>())
            ("c,calculator", "Set Calculator", cxxopts::value<std::string>())
            ("l,logLevel", "Set log level", cxxopts::value<std::string>())
            ("p, particleContainer", "Set particle container", cxxopts::value<std::string>())
            ("s, domainSize" , "Set domain size", cxxopts::value<std::vector<double>>()->default_value("180,90,1"))
            ("r, cutoffRadius", "Set cutoff radius", cxxopts::value<double>()->default_value("3."))

    ;




    auto parseResult = options.parse(argc, argv);

    //if the argument help or h is read print the help message
    if (parseResult.count("help")) {
        printHelp();
        return false;
    }

    if (parseResult.count("logLevel")) {
        std::string level = parseResult["logLevel"].as<std::string>();
        if (level == "off") {
            spdlog::set_level(spdlog::level::off);
        } else if (level == "trace") {
            spdlog::set_level(spdlog::level::trace);
        } else if (level == "debug") {
            spdlog::set_level(spdlog::level::debug);
        } else if (level == "info") {
            spdlog::set_level(spdlog::level::info);
        } else if (level == "warn") {
            spdlog::set_level(spdlog::level::warn);
        } else if (level == "error") {
            spdlog::set_level(spdlog::level::err);
        } else if (level == "all") {
            spdlog::set_level(spdlog::level::trace);
        } else {
            SPDLOG_ERROR("Erroneous programme call! Invalid log level");
            printHelp();
            return false;
        }
        SPDLOG_DEBUG("log level was set to {}", level);
    }


    //set the input file
    if (parseResult.count("input")) {
        inputFile = parseResult["input"].as<std::string>();
        SPDLOG_DEBUG("Input file is: {}", inputFile);
    }

    //check if the input file ends with ".txt"
    if (inputFile.length() < 5 || (inputFile.compare(inputFile.length() - 4, 4, ".txt") != 0 && inputFile.compare(inputFile.length() - 4, 4, ".xml") != 0)) {
        SPDLOG_ERROR("Invalid input path! Input file must be a '.txt' file");
        printHelp();
        return false;
    }

    //check if the input file exists
    std::ifstream file(inputFile);
    if (!file) {
        SPDLOG_ERROR("Failed to open input file {}, check if the spelling is correct!", inputFile);
        printHelp();
        return false;
    }

    file.seekg(0, std::ios::end);
    if (file.tellg() == 0) {
        SPDLOG_ERROR("Failed to read input file {}! Input file is empty", inputFile);
        printHelp();
        return false;
    }
    file.close();

    auto domainSize = parseResult["domainSize"].as<std::vector<double>>();
    if (domainSize.size() != 3) {
        SPDLOG_ERROR("Invalid domain size. Domain size must be 3. For a 2d simulation please set the last dimension to 1");
        printHelp();
        return false;
    }
    //SPDLOG_DEBUG("Domain size array is: {}", domainSize);
    std::array<double,3> domainSizeArray = {domainSize[0], domainSize[1], domainSize[2]};

    if (parseResult["cutoffRadius"].as<double>() <= 0) {
        SPDLOG_ERROR("Cutoff radius must be greater than zero");
        printHelp();
        return false;
    }

    double cutoffRadius = parseResult["cutoffRadius"].as<double>();
    SPDLOG_DEBUG("Cutoff radius is: {}", cutoffRadius);


    if (parseResult.count("particleContainer")) {
        std::string containerType = parseResult["particleContainer"].as<std::string>();
        if (containerType == "DSC") {
            particleContainer = std::make_unique<ParticleContainers::DirectSumContainer>();
        } else if (containerType == "LCC") {
            particleContainer = std::make_unique<ParticleContainers::LinkedCellContainer>(domainSizeArray, cutoffRadius);
        } else {
            SPDLOG_ERROR("Invalid container type!");
            printHelp();
            return false;
        }
    }



    //set deltaT and endTime
    if (parseResult["deltaT"].as<double>() <= 0.0) {
        SPDLOG_ERROR("Invalid deltaT value. DeltaT has to be positive");
        printHelp();
        return false;
    }

    if (parseResult["endTime"].as<double>() <= 0.0) {
        SPDLOG_ERROR("Invalid endTime value. EndTime has to be positive");
        printHelp();
        return false;
    }

    deltaT = parseResult["deltaT"].as<double>();
    endTime = parseResult["endTime"].as<double>();
    outputWriter = std::make_unique<outputWriters::VTKWriter>();
    calculator = std::make_unique<Calculators::GravityCalculator>();


    //set the output writer
    if (parseResult.count("output")) {
        std::string outputWriterTemp = parseResult["output"].as<std::string>();
        if (outputWriterTemp == "VTK") {
            outputWriter = std::make_unique<outputWriters::VTKWriter>();
            SPDLOG_DEBUG("{} is selected as the output writer", outputWriterTemp);
        } else if (outputWriterTemp == "XYZ") {
            outputWriter = std::make_unique<outputWriters::XYZWriter>();
            SPDLOG_DEBUG("{} is selected as the output writer", outputWriterTemp);
        } else {
            SPDLOG_ERROR("Erroneous programme call! Invalid output writer specified!");
            printHelp();
            return false;
        }
    }

    //set the calculator
    if (parseResult.count("calculator")) {
        std::string calculatorTemp = parseResult["calculator"].as<std::string>();
        if (calculatorTemp == "LJC") {
            calculator = std::make_unique<Calculators::LennardJonesCalculator>();
            SPDLOG_INFO("{} is selected as the calculator", calculatorTemp);
        } else if (calculatorTemp == "Default") {
            calculator = std::make_unique<Calculators::GravityCalculator>();
            SPDLOG_INFO("{} is selected as the calculator", calculatorTemp);
        } else {
            SPDLOG_ERROR("Erroneous programme call! Invalid calculator specified!");
            printHelp();
            return false;
        }
    }

    
    return true;
}


void MolSim::runSim(ParticleContainers::ParticleContainer &particleContainer, double &deltaT, double &endTime, int &freq,
                    std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                    std::unique_ptr<Calculators::Calculator> &calculator) {            


    SPDLOG_INFO("REACHED 1");
    const std::string outName = "MD";

    double currentTime = 0.0;
    int iteration = 0;

    SPDLOG_INFO("REACHED 2");

    while (currentTime < endTime) {
        SPDLOG_INFO("REACHED 3");

        calculator->calculateXFV(particleContainer, deltaT);

        SPDLOG_INFO("REACHED 4");

        iteration++;

        if (iteration % freq == 0) {
            outputWriter->plotParticles(iteration, particleContainer, outName);
        }

        SPDLOG_DEBUG("Iteration {} finished.", iteration);
        currentTime += deltaT;
    }
    SPDLOG_INFO("Output written. Terminating...");
}

