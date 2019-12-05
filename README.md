## Installation

* Clone and Install the library `git clone https://github.com/LeMonADE-project/LeMonADE.git`
* Install cmake (minimum version 2.8)
* Go to one of the project directories and perform the standard build of LeMonADE
* This directory can be compiled by
 
````sh
    # prepare the build directory
    mkdir build
    cd build
    # run cmake with the lemonade path
    cmake -DLEMONADE_DIR=</path/to/LeMonADE-library/>  ..
    make
````
* There is an option -DLEMONADE_TESTS which can be either ON or OFF and does compile the tests
 in the test directory. Please write tests for your code to verify the correctness!!! This 
 helps to 

## Examples

* in analyzers and updaters examples files can be found
* the are used in [the example project](projects/exampleProject/)

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

 

