//
// Created by farouk on 26/06/23.
//
#include <iostream>
#include <omp.h>
#include <fstream>
#include <chrono>
#include "EC.h"
#define NB_THREADS 8


// TODO : do not assume that the os will give you all the threads you demanded
EdgeCentric parallelGraphCreation(const std::string& path) {

    auto start = std::chrono::high_resolution_clock::now();
    EdgeCentric graphs[NB_THREADS];

    int nb_edges ;
    std::ifstream conf(path+".conf");
    conf >> nb_edges ;

    auto step = nb_edges / NB_THREADS;

    int offsets[NB_THREADS+1] ;
    offsets[0] =0 ;
    for (int i = 1; i < NB_THREADS; ++i) {
        offsets[i]  = offsets[i-1]+step ;
    }
    offsets[NB_THREADS] =nb_edges ;

    for (int i = 0; i <=NB_THREADS ; ++i) {
        std::cout << offsets[i] << " "  ;
    }
    std::cout << "\n" ;

#pragma omp parallel
    {
        auto thread_id = omp_get_thread_num();
        std::ifstream edge_list(path+".bin");

        // 8 is 2 integers in each line and each integer is 4 bytes
        edge_list.seekg(8*offsets[thread_id], std::ios::beg);

        int a, b ;
        edge_list.read((char *)&a,sizeof(int) );
        edge_list.read((char *)&b,sizeof(int) );
        graphs[thread_id].src.push_back(a);
        graphs[thread_id].count.push_back(1);
        graphs[thread_id].dst.push_back(b);
        for (int j = offsets[thread_id]+1; j < offsets[thread_id+1] ; ++j) {
            // TODO : store prev variable better than accessing last element of the vector each time
            // TODO : collect metadata of the graph in the pass and store them somewhere
            edge_list.read((char *)&a,sizeof(int) );
            edge_list.read((char *)&b,sizeof(int) );
            if (graphs[thread_id].src.back() != a) {
                graphs[thread_id].src.push_back(a);
                graphs[thread_id].count.push_back(1);
            } else {
                graphs[thread_id].count.back()++;
            }

            graphs[thread_id].dst.push_back(b);
        }
        edge_list.close();
    }

    /*
    * TODO : change it to this and compare implementation
    * merge step : allocate a fixed sized array as the sum of all arrays
    * memcpy() content of each vector to the array and free the memory used by the vector
    * */

    // PFA team method
    for(int i = 1; i < NB_THREADS ; ++i) {
        graphs[0].src.insert(graphs[0].src.end(), graphs[i].src.begin(), graphs[i].src.end());
        graphs[0].count.insert(graphs[0].count.end(), graphs[i].count.begin(), graphs[i].count.end());
        graphs[0].dst.insert(graphs[0].dst.end(), graphs[i].dst.begin(), graphs[i].dst.end());
        graphs[i].src.clear();
        graphs[i].dst.clear();
        graphs[i].count.clear();
        graphs[i].src.shrink_to_fit();
        graphs[i].dst.shrink_to_fit();
        graphs[i].count.shrink_to_fit();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "creating EC in parallel took : " << duration.count() << '\n' ;

    return graphs[0] ;
}
