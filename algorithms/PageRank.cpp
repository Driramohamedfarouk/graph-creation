//
// Created by farouk on 30/06/23.
//
#include <iostream>
#include "../graph-creation/EC.h"
#include "PageRank.h"
#include <cstring>
#include <fstream>
#include <chrono>

void parallelPageRank(const std::string& path,const int n,const int nb_iteration){
    // create edge centric format and an array of size n containing out_deg
    ExtendedEdgeCentric g ;
    g = createGraphFromFilePageRank(path,n);

    // allocate two arrays of size n
    // one contains PR for previous iteration and one contains PR for current iteration
    std::cout << "loaded the graph in memory " << '\n' ;
    auto start = std::chrono::high_resolution_clock::now();
    double* arr1;
    arr1 = static_cast<double *>(malloc(n * sizeof(double)));


    double* arr2;
    arr2 = static_cast<double *>(malloc(n * sizeof(double)));


    double* previousPR = arr1 ;
    double* PR = arr2 ;
    double* temp ;

    // damping factor of pagerank
    const float d =0.85;

    memset(previousPR,1.0f,n*sizeof(double ));

    for (int i = 0; i < nb_iteration; ++i) {
        memset(PR, 0, n*sizeof(double ));

        // in a fully parallel manner update previous to previous[i] /= out_degree[i]
#pragma omp parallel for
        for (int j = 0; j < n; ++j) {
            // verticies with outdgeree zero wel not be present in the EC
            previousPR[j] /= (double)g.out_degree[j];
        }

        // partition the source array
        // the performance of parallel pageRank depends on the partitioning of edges
        // for each source add the value of previous[source] to count[source] next destinations
#pragma omp parallel for schedule(dynamic,10)
        for (int j = 0; j < g.src.size() ; ++j) {
            // internal loop should not be done in parallel
            // too much random access here
            for (int k = g.count[j]; k < g.count[j+1] ; ++k) {
                // not exactly k : convert to offset
                PR[g.dst[k]]+=previousPR[g.src[j]] ;
            }

        }

        // multiply in a fully parallel manner pageRank vector by alpha and add 1-alpha to it
#pragma omp parallel for
        for (int j = 0; j < n; ++j) {
            PR[j] = d*PR[j] + (1 -d)/(float)n ;
        }

        // previous = pageRank && reinitialize pageRank
        // pointer to previous now points to PR
        // memset PR to all zeros
        temp = previousPR ;
        previousPR = PR ;
        PR  =  temp ;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "calculating pageRank took : " << duration.count() << '\n' ;
    std::ofstream out("/home/farouk/CLionProjects/untitled/PageRankOutput.txt");
    for (int j = 0; j < n; ++j) {
        out << PR[j]  ;
    }
    out.close();
    /*
    delete PR ;
    delete previousPR ;
    delete arr1 ;
    delete arr2 ;*/
}


