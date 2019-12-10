## ###############  add your executables here and link them to LeMonADE ############# ##
add_executable(linearChainIdeal lemonade_example_main.cpp)
target_link_libraries(linearChainIdeal LeMonADE)

add_executable(linearChainReal lemonade_example_main_exclvol.cpp)
target_link_libraries(linearChainReal LeMonADE)

add_executable(linearChainSolvent lemonade_example_main_nninter.cpp)
target_link_libraries(linearChainSolvent LeMonADE)
