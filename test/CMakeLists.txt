cmake_minimum_required (VERSION 2.8)

## ###############  Tests linked to main  ############# ##
add_executable(testsLeMonADE test_main.cpp test_updaterExample.cpp)
target_link_libraries(testsLeMonADE LeMonADE )
