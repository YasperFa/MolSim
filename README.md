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


        With Doxygen:    'cmake .. -D DOXY=1' (default)
   
        Without Doxygen: 'cmake .. -D DOXY=0' 
   
        With Tests:      'cmake ..  -DBUILD_TESTS=ON' (default)

        Without Tests:   'cmake ..  -DBUILD_TESTS=OFF'

The value of DOXY is saved in the cache, so for repeated calls, 'cmake ..' is enough.
   
Doxygen is automatically disabled if the Doxygen executable can not be found.

Tests are automatically enabled if only cmake .. is used

---
3) Make:

        'make'

The user should ensure they are in the directory where they built the project with cmake.

---
4) Running the code:


        './MolSim .{INPUT_PATH} -c {CALCULATOR} -d {DELTA_T} -e {END_TIME} {OUTPUT_WRITER}'

Example calls: 

        './MolSim -h' or './MolSim --help'
        './MolSim ../input/eingabe-sonne.txt -c default -vtk'
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

---
## Creating Doxygen Documentation:

After running cmake with Doxygen enabled (see 2.):

        'make doc_doxygen'

If running this command and doxygen is disabled, nothing will happen.

The output is in the directory ../doxys_documentation.

---
## Executing tests:
      
       After running cmake with tests enabled (see 2.), and then running make:

The executable is in the folder build/tests. For execution either navigate to the folder tests and run 

      ./Test
or run 

      ctest / ctest -V 
(for detailed execution) in build directory

        
       

