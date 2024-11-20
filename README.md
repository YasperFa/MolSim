\mainpage Molecular Dynamics WS24 - GroupG

dummy change will be deleted
 
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
4) Running the code: ** values from the command line will be overwritten by the values specified in the xml file if the latter is specified as input file **


        './MolSim -i .{INPUT_PATH} -c {CALCULATOR} -d {DELTA_T} -e {END_TIME} -o {OUTPUT_WRITER} -l {LOG_LEVEL}'
or

        './MolSim --input=.{INPUT_PATH} --calculator={CALCULATOR} --deltaT={DELTA_T} -endTime={END_TIME} --output={OUTPUT_WRITER} --logLevel {LOG_LEVEL}'


Example calls: 

        './MolSim -h' or './MolSim --help'
        './MolSim -i ../input/eingabe-sonne.txt -c Default -o VTK -l debug'
        './MolSim -i ../input/schema.xml'
        './MolSim -i ../input/schema.xml -c Default -o VTK -l debug'
        './MolSim --input=../input/eingabe-sonne.txt --calculator=Default --deltaT=0.014 --endTime=1000 --output=XYZ --logLevel=info'
        './MolSim -i ../input/cuboid-example.txt -c LJC -o VTK -d 0.0002 -e 5'

The output should be in the build directory.    
    
---
5) Arguments:

    Compulsory arguments:

        '{INPUT_PATH}': Path to the input file. For example, '-i ../input/eingabe-sonne.txt' or 'input=../input/cuboid-example.txt' or s'input=../input/schema.xml'.

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

        
       

