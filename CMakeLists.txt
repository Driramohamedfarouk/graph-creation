cmake_minimum_required(VERSION 3.24)
project(graph_processing_framework)

# std=c++17
set(CMAKE_CXX_STANDARD 17)

#-fsanitize=address

# added openMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -O3 -mavx -march=native -fopenmp -fcilkplus")

add_executable(ConvertToBinary utils/ConvertToBinary.h utils/ConvertToBinary.cpp  )

add_executable(SplitConvertToBinary utils/SplitConvertToBinary.cpp )

#add_executable(csc2b utils/CompressedSplitConvertToBinary.cpp)

add_executable(PageRank utils/timer.h utils/pv_vector.h algorithms/PageRank.cpp graph-creation/EC.h graph-creation/EC.cpp graph-creation/getDstFile.h graph-creation/getDstFile.cpp)

add_executable(BetaPageRank utils/timer.h utils/pv_vector.h algorithms/BetaPageRank.cpp graph-creation/EC.h graph-creation/EC.cpp graph-creation/getDstFile.h graph-creation/getDstFile.cpp)

add_executable(bfs graph-creation/getDstFile.h graph-creation/getDstFile.cpp graph-creation/ChainedEC.h graph-creation/ChainedEC.cpp algorithms/bfs.cpp )

#add_executable(cec graph-creation/ChainedEC.h graph-creation/ChainedEC.cpp)

add_executable(OneStepNeighbour algorithms/OneStepNeighbour.cpp graph-creation/ChainedEC.cpp graph-creation/ChainedEC.h graph-creation/getDstFile.h graph-creation/getDstFile.cpp )

add_executable(test_bfs test/bfs.cpp)

add_executable(CC graph-creation/EC.cpp graph-creation/getDstFile.cpp algorithms/CC.cpp)

add_executable(test_bc graph-creation/EC.cpp graph-creation/getDstFile.cpp test/bc.cpp)

add_executable(hops graph-creation/getDstFile.cpp graph-creation/ChainedEC.cpp algorithms/count_hops.cpp)

add_executable(exp_bfs experimental-features/top_down_bfs/ChainedEC.h experimental-features/top_down_bfs/ChainedEC.cpp experimental-features/top_down_bfs/bfs.cpp)

add_executable(exp_dobfs experimental-features/do_bfs/ChainedEC.h experimental-features/do_bfs/ChainedEC.cpp experimental-features/do_bfs/bfs.cpp)

add_executable(sort experimental-features/in_mem_sort.cpp)