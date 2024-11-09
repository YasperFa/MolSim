//
// Created by Celik Yucel on 08.11.24.
//
#pragma once

#include <iostream>

#include "Calculator/Calculator.h"
#include "Calculator/lennardJonesCalculator.h"
#include "FileReader.h"
#include "Objects/ParticleContainer.h"
#include "outputWriter/OutputWriter.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/XYZWriter.h"
#include "spdlog/spdlog.h"
#include "cxxopts.hpp"

class MolSim {
public:

    static void printHelp() {
        std::cout << R"(
    Welcome to MolSim helper!
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


    static bool parseArguments(int argc, char *argv[], std::string &inputFile, double &deltaT, double &endTime,
                               std::unique_ptr<outputWriters::OutputWriter> &outputWriter,
                               std::unique_ptr<Calculators::Calculator> &calculator) {

        cxxopts::Options options("MolSim", "Molecular Simulation Of Group G WS24");

        options.add_options()
                ("help,h", "print help message")
                ("input, i", "input file path", cxxopts::value<std::string>())
                ("deltaT, d", "Set deltaT", cxxopts::value<double>()->default_value("0.014"))
                ("endTime, e", "Set endTime", cxxopts::value<double>()->default_value("1000"))
                ("output, o", "Set Outputwriter (VTK or XYZ)", cxxopts::value<std::string>())
                ("calculator, c", "Set Calculator", cxxopts::value<std::string>())
                ("logLevel, l", "Set log level", cxxopts::value<std::string>()->default_value("info"));


        auto res = options.parse(argc, argv);

        //if the argument help or h is read print the help message
        if (res.count("help")) {
            printHelp();
            return false;
        }

        //set the input file
        if (res.count("input")) {
            inputFile = res["input"].as<std::string>();
            SPDLOG_INFO("Input file is: {}", inputFile);
        }

        //check if the input file ends with ".txt"
        if (inputFile.length() < 5 || inputFile.compare(inputFile.length() - 4, 4, ".txt") != 0) {
            SPDLOG_ERROR("Invalid input path! Input file must be a '.txt' file");
            printHelp();
            return false;
        }

        //check if the input file exists
        std::ifstream file(inputFile);
        if(!file) {
            SPDLOG_ERROR("Failed to open input file {}, check if the spelling is correct!", inputFile);
            printHelp();
            return false;
        }
        file.close();

        //check if the input file is empty
        if (inputFile.empty()) {
            SPDLOG_ERROR("Input file is empty.");
            printHelp();
            return false;
        }


        //set deltaT and endTime
        deltaT = res["deltaT"].as<double>();
        endTime = res["endTime"].as<double>();
        outputWriter = std::make_unique<outputWriters::VTKWriter>();
        calculator = std::make_unique<Calculators::Calculator>();




        //set the outpit writer
        if (res.count("output")) {
            std::string outputWriterTemp = res["output"].as<std::string>();
            if (outputWriterTemp == "VTK") {
                outputWriter = std::make_unique<outputWriters::VTKWriter>();
                SPDLOG_INFO("{} is selected as the output writer", outputWriterTemp);
            } else if (outputWriterTemp == "XYZ") {
                outputWriter = std::make_unique<outputWriters::XYZWriter>();
                SPDLOG_INFO("{} is selected as the output writer", outputWriterTemp);
            } else {
                SPDLOG_ERROR("Erroneous programme call! Invalid output writer specified!");
                printHelp();
                return false;
            }
        }

        //set the calculator
        if (res.count("calculator")) {
            std::string calculatorTemp = res["calculator"].as<std::string>();
            if (calculatorTemp == "LJCalculator") {
                calculator = std::make_unique<Calculators::lennardJonesCalculator>();
                SPDLOG_INFO("{} is selected as the calculator", calculatorTemp);
            } else if (calculatorTemp == "Default") {
                calculator = std::make_unique<Calculators::Calculator>();
            } else {
                SPDLOG_ERROR("Erroneous programme call! Invalid calculator specified!");
                printHelp();
                return false;
            }
        }

        if (res.count("logLevel")) {
            std::string level = res["logLevel"].as<std::string>();
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

        return true;
    }


    static void runSim(ParticleContainer& particleContainer, double& deltaT, double& endTime,
        std::unique_ptr<outputWriters::OutputWriter>& outputWriter, std::unique_ptr<Calculators::Calculator>& calculator) {
        SPDLOG_INFO("Hello from MolSim for PSE!");

        const std::string outName = "MD";

        double currentTime = 0.0;
        int iteration = 0;

        while (currentTime < endTime) {
            calculator->calculateXFV(particleContainer, deltaT);

            iteration++;
            if (iteration % 10 == 0) {
                outputWriter->plotParticles(iteration, particleContainer, outName);
            }

            SPDLOG_TRACE("Iteration {} finished.", iteration);
            currentTime += deltaT;
        }
        SPDLOG_INFO("Output written. Terminating...");
    }
};
