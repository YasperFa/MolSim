\mainpage Molecular Dynamics WS24 - GroupG

 
===
## Prerequisites
To be able to build and run this code, the following tools have to be installed:

- cmake
- g++
- libxerces-c-dev
(-Optional: Doxygen)

## Building and Running
1) Create a 'build' directory and switch to the newly created directory:


        'mkdir build && cd build'
---
2) Build the code by using the following command:

        'cmake ..'

    Using optional arguments: (if an argument is not specified, the default value is used)

    Doxygen:    With Doxygen:    'cmake .. -D DOXY=ON'   (default)
                Without Doxygen: 'cmake .. -D DOXY=OFF' 
    
                Doxygen is automatically disabled if the Doxygen executable can not be found.

    Tests:      With Tests:      'cmake ..  -DBUILD_TESTS=ON'   (default)
                Without Tests:   'cmake ..  -DBUILD_TESTS=OFF'

    Logging:    'cmake .. -D LVL={LOG_LEVEL}'           (default = 'INFO')

                Possible values, in ascending order: 'OFF', 'ERROR', 'WARN', 'INFO', 'DEBUG', 'TRACE', 'ALL'
                'ALL' is an alias of 'TRACE' and enables all possible logs.
                The compile-time log level can be overridden with a lower log level at runtime (see 5. for details).

    Note that cmake variables are saved in the cache and therefore do not automatically reset to the default value.
    It is possible to combine several optional arguments.

---
3) Make:

        'make'

The user should ensure they are in the directory where they built the project with cmake.

---
4) Running the code:


        './MolSim .{INPUT_PATH} -c {CALCULATOR} -d {DELTA_T} -e {END_TIME} {OUTPUT_WRITER} -l {LOG_LEVEL}'

Example calls: 

        './MolSim -h' or './MolSim --help'
        './MolSim ../input/eingabe-sonne.txt -c default -vtk -l debug'
        './MolSim ../input/eingabe-sonne.txt -c default -d 0.014 -e 1000 -xyz'
        './MolSim ../input/cuboid-example.txt -c LJCalculator -vtk -d 0.0002 -e 5'

The output should be in the build directory.    
    
---
5) Arguments:

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

---
## Creating Doxygen Documentation:

After running cmake with Doxygen enabled (see 2.):

        'make doc_doxygen'

If running this command and doxygen is disabled, nothing will happen.

The output is in the directory ../doxys_documentation.

---
## Executing tests:
      
       After running cmake with tests enabled (see 2.), and then running make:

The executable is in the folder build. For execution either run:

      ./Test
or run 

      ctest / ctest -V 
(for detailed execution) in build directory

        
       

