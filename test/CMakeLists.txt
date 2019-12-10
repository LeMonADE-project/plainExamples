## Tests need to be linke to the main test file like
#add_executable(testsLeMonADE test_main.cpp test_A.cpp test_B.cpp ...)

## ###############  Tests linked to main  ############# ##
add_executable(testsLeMonADE test_main.cpp test_updaterExample.cpp)
target_link_libraries(testsLeMonADE LeMonADE )
