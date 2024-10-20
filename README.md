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


    './MolSim .{INPUT_PATH}'
    Example call: './MolSim ../input/eingabe-sonne.txt'
---
5) Arguments:


    '{INPUT_PATH}': path to the input file. For example, './input/eingabe-sonne.txt'.
