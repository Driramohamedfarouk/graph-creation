cmake_minimum_required(VERSION 3.24)
project(graph_processing_framework)

set(CMAKE_CXX_STANDARD 17)

# added openMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp")

add_executable(ConvertToBinary utils/ConvertToBinary.h utils/ConvertToBinary.cpp  )
add_executable(SplitConvertToBinary utils/SplitConvertToBinary.cpp )
#add_executable(csc2b utils/CompressedSplitConvertToBinary.cpp)

add_executable(PageRank algorithms/PageRank.cpp graph-creation/EC.h graph-creation/EC.cpp)
#add_executable(bfs algorithms/bfs.cpp graph-creation/chainedEC.h graph-creation/chainedEC.cpp )

#add_executable(cec graph-creation/ChainedEC.h graph-creation/ChainedEC.cpp)

add_executable(OneStepNeighbour algorithms/OneStepNeighbour.cpp graph-creation/ChainedEC.cpp graph-creation/ChainedEC.h )