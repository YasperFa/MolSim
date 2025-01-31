\mainpage Molecular Dynamics WS24 - GroupG
 

===
## Prerequisites
To be able to build and run this code, the following tools have to be installed:

- cmake
- g++
- libxerces-c-dev
(-Optional: Doxygen)

---
## Cluster
To be able to build and run this code on the cluster, do the following:
 - module load user_spack
 - spack install xerces-c
 - module load xerces-c/3.2.1
 Note: when compiling use -DBUILD_TESTS=OFF to disable the tests from compiling, otherwise it won't work.
 For xml input, the 'xsi:noNamespaceSchemaLocation'-path has to be adjusted depending on the directory in which the code is executed

 ---
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


        './MolSim -i .{INPUT_PATH} -c {CALCULATOR} -p {PARTICLE_CONTAINER} -d {DELTA_T} -e {END_TIME} -o {OUTPUT_WRITER} -l {LOG_LEVEL} -s {DOMAIN_SIZE} -r {CUTOFF_RADIUS} -b {BOUNDARY_CONDITION} -g {GRAVITY}
or

        './MolSim --input=.{INPUT_PATH} --calculator={CALCULATOR} --particleContainer={PARTICLE_CONTAINER} --deltaT={DELTA_T} -endTime={END_TIME} --output={OUTPUT_WRITER} --logLevel={LOG_LEVEL}' --domainSize={DOMAIN_SIZE} --cutoffRadius={CUTOFF_RADIUS} --boundaryCondition={BOUNDARY_CONDITION} --g={GRAVITY}


Example calls: 

        './MolSim -h' or './MolSim --help'
        './MolSim -i ../input/eingabe-sonne.txt -c Default -o VTK -l debug -p DSC'
        './MolSim -i ../input/schema.xml'
        './MolSim -i ../input/schema.xml -c Default -o VTK -l debug'
        './MolSim --input=../input/eingabe-sonne.txt --calculator=Default --deltaT=0.014 --endTime=1000 --output=XYZ --logLevel=info --particleContainer=DSC'
        './MolSim -i ../input/cuboid-example.txt -c LJC -o VTK -d 0.0002 -e 5 -p DSC'
        './MolSim -i ../input/disc-example.txt -c LJC -o VTK -d 0.00005 -e 10 -p LCC -r 3.0 -s 120,50,1 -b o,o,o,r,o,o'

The output should be in the build directory.    
    
---
5) Arguments:

    Compulsory arguments:

        '{INPUT_PATH}': Path to the input file. For example, '-i ../input/eingabe-sonne.txt' or 'input=../input/cuboid-example.txt' or s'input=../input/schema.xml'.

        '{OUTPUT_WRITER}': Specifies which output writer will be used. Either VTK or XYZ has to be chosen. Examples: "-o VTK" or "-output XYZ"

        '{PARTICLE_CONTAINER}': Specifies what kind of particle container to use. Either LCC (LinkedCellContainer) or DSC (DirectSumContainer) has to be chosen. Examples: "-p LCC" or "-particleContainer DSC".

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
        The argument has to be passed with the following format: '-l {logLevel}' or '--logLevel {logLevel}'. 

        '{DOMAIN_SIZE'}: The size of the domain used. This is only effective when using LCC as particle container. The domain size must consist of three positive values seperated by commas. If no domain size is specified and LCC is chosen, a default size of 180,90,1 will be used. 
        The argument has to be passed with the following format: '-s {domainSize} or '--domainSize {domainSize}', 
        where domain has the format {x},{y},{z}.

        '{CUTOFF_RADIUS}': The cutoff radius that will be used by the LinkedCellContainer. The argument has to be passed with a positive number
        following the format: '-r {radius}' or '--cutoffRadius {radius}'. If no radius is specified, a default radius of 3 will be used.

        '{BOUNDARY_CONDITION}': The boundary condition that will be used by the LinkedCellContainer. Setting this when LCC is not selected will cause an error. The boundary condition consists of six values seperated by commas, each of them determines one boundary and possible values are o for outflow, r for reflecting and p for periodic. The argument has to be passed with the following format: '-b {boundaryCondition}' or '--boundaryCondition {boundaryCondition}', where boundaryCondition has the following format: {left},{right},{top},{bottom},{front},{back}. If no value is specified, outflow will be used for all boundaries.

        '{GRAVITY}': The gravitatonal acceleration in y-direction. Passed as a positive or negative double value. If no value is given, the defult value of 0 will be used.

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

        
       

