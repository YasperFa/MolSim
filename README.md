\mainpage
# Molecular Dynamics WS24 - GroupG
===

## Installation
### Prerequisites
To be able to build and run this code, the following tools have to be installed:

- cmake
- g++
- libxerces-c-dev
(-Optional: Doxygen)

### Building and Running
1) Create a 'build' directory and switch to the newly created directory:


    'mkdir build && cd build'
---
2) Build the code by using the following command:

    'cmake ..'

    With Doxygen:    'cmake .. -D DOXY=1' (default)
    Without Doxygen: 'cmake .. -D DOXY=0' 

    (The value of DOXY is saved in the cache, so for repeated calls 'cmake ..' is enough.
    Doxygen is automatically disabled if the Doxygen executable can not be found.)
    
---
3) Make:


    'make'
---
4) Running the code:


    './MolSim .{INPUT_PATH}'
    Example call: './MolSim ../input/eingabe-sonne.txt'
---
5) Arguments:


    '{INPUT_PATH}': path to the input file. For example, './input/eingabe-sonne.txt'.

6) Creating Doxygen Documentation:

    'make doc_doxygen'

    (If running this command and doxygen is disabled, nothing will happen.)

