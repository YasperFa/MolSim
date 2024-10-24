# Molecular Dynamics WS24 - GroupG
===

## Installation
### Prerequisites
To be able to build and run this code, the following tools have to be installed:

- cmake
- g++
- libxerces-c-dev

### Building and Running
1) Create a 'build' directory and switch to the newly created directory:


    'mkdir build && cd build'
---
2) Build the code by using the following command:


    'cmake ..'
---
3) Make:


    'make'
---
4) Runing the code:


    './MolSim .{INPUT_PATH} {DELTA_T} {END_TIME} {OUTPUT_WRITER}'

Example calls: 

    './MolSim -h' or './MolSim --help'
    './MolSim ../input/eingabe-sonne.txt -vtu'
    './MolSim ../input/eingabe-sonne.txt -d 0.014 -e 1000 -xyz'
    
---
5) Arguments:

Compulsory arguments:

    '{INPUT_PATH}': path to the input file. For example, './input/eingabe-sonne.txt'.

    '{OUTPUT_WRITER}': specifies which output writer will be used. Either -vtk or -xyz has to be choosen.

Optional arguments:

    '{DELTA_T}': Time step which will be used for the simulation. The argument has to be passed with a positiv number
    following the format: '-d {positive number}'.
    If -d is not specified while executing the program, the default value d = 0,014 will be used.

    '{END_TIME}': The end time which will be used for the simulation. The argument has to be passed with a positiv number
    following the format: '-e {positive number}'.
        If -e is not specified while executing the program, the default value e = 1000 will be used.
    

---


