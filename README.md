# plainExamples

## Installation

* Clone and Install the library `git clone https://github.com/LeMonADE-project/LeMonADE.git`
* Install cmake (minimum version 2.8)
* This directory can be compiled by
 
````sh
    # prepare the build directory
    mkdir build
    cd build
    # run cmake with the lemonade path
    cmake -DLEMONADE_DIR=</path/to/LeMonADE-library/>  ..
    make
````
* See Tests section for turn on tests.
## Examples
* can be found in the [project folder](projects)

### exampleProject
* simple template introducing analyzers and updaters
* provides the main to start with

### examplesLinearChains:lemonade_example_main.cpp
* creation and simulation of a single polymer chain
* no excluded volume condition == ideal case

### examplesLinearChains:lemonade_example_main_exclvol.cpp
* creation and simulation of a single polymer chain
* with excluded volume condition == real case

### examplesLinearChains:lemonade_example_main_nninter.cpp
* creation and simulation of a single polymer chain and explicit solvent
* with excluded volume and interaction between polymer and solvent -> poor solvent condition

### exampleConnection:lemonade_example_main_exvol_crosslinking.cpp
* creation and simulation of a solution of polymer chains with explicit cross-linker
* with excluded volume condition
* chain ends can be cross-linked with tetra-functional cross-linker -> end-linked polymer network


## Tests

* in [the test directory](test/) you find a setup using the [catch2](https://github.com/catchorg/Catch2) framework 
* There is an option -DLEMONADE_TESTS which can be either ON or OFF and does compile the tests
 in the test directory. The default is setting tests to OFF. To turn them on use the option when executing cmake 
`````sh
    cmake -DLEMONADE_DIR=</path/to/LeMonADE-library/> -DLEMONADE_TESTS=ON  ..
`````
  The executable can be found in /path/to/build/bin/ and run them by 
`````sh  
  cd /path/to/build/bin/ 
  ./testsLeMonADE
  `````
* Please write tests although it might appear annoying but it helps you to find nasty bugs and tests the correctness of your algorithms!

## Contributions

* Any contributions making the programm better are welcome as well as hints for bugs.
* Every contribution should be made by following the guidelines made by the [gitflow workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow)

 

