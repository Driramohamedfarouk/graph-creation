cmake_minimum_required(VERSION 3.24)
project(graph_processing_framework)

set(CMAKE_CXX_STANDARD 17)

# added openMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(ConvertToBinary utils/ConvertToBinary.h utils/ConvertToBinary.cpp  )
add_executable(SplitConvertToBinary utils/SplitConvertToBinary.cpp )
add_executable(PageRank algorithms/PageRank.cpp graph-creation/EC.h graph-creation/EC.cpp utils/util.h utils/util.cpp)
add_executable(bfs algorithms/bfs.cpp graph-creation/chainedEC.h graph-creation/chainedEC.cpp utils/util.h utils/util.cpp )
#add_executable(gpf main.cpp graph-creation/EC.cpp  graph-creation/parallelEC.cpp )
