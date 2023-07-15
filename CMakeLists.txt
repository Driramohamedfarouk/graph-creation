cmake_minimum_required(VERSION 3.24)
project(graph_processing_framework)

set(CMAKE_CXX_STANDARD 17)

# added openMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

#add_executable(ConvertToBinary utils/ConvertToBinary.cpp graph-creation/parallelEC.cpp)
#add_executable(PageRank algorithms/PageRank.cpp)
add_executable(gpf main.cpp graph-creation/EC.cpp utils/util.h utils/util.cpp algorithms/PageRank.h main.cpp algorithms/PageRank.cpp graph-creation/parallelEC.cpp utils/ConvertToBinary.cpp graph-creation/chainedEC.h graph-creation/chainedEC.cpp)
