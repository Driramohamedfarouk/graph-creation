//
// Created by farouk on 30/06/23.
//
#include <iostream>
#include "../graph-creation/EC.h"
#include "PageRank.h"
#include <chrono>
#include <algorithm>
#include <fstream>
#include <numeric>
#include "../graph-creation/getDstFile.h"

void parallelPageRank(const std::string& path, const int n, const int nb_iteration){


    auto x = getDstFile(path);
    int *dst = x.first ,nb_edges = x.second ;

    ExtendedPairEdgeCentric g = BranchlessCreateGraphFromFilePageRank(path,n,nb_edges);

    std::cout << "loaded the graph in memory " << '\n' ;

    auto start = std::chrono::high_resolution_clock::now();

    // allocate two arrays of size n
    // one contains PR for previous iteration
    // and one contains PR for current iteration
    auto* previousPR = static_cast<double *>(malloc(n * sizeof(double)));
    auto* PR = static_cast<double *>(malloc(n * sizeof(double)));
    double* temp ;

    // damping factor of pagerank
    const float d =0.85;

    for (int i = 0; i < n; ++i) {
        previousPR[i] = 1.0/n ;
    }

    for (int i = 0; i < nb_iteration; ++i) {

        for (int  j = 0; j < n; ++j) {
            PR[j] = 0.0 ;
        }

        // in a fully parallel manner update previous to previous[i] /= out_degree[i]
//        #pragma omp parallel for
//        for (int j = 0; j < n; ++j) {
//            previousPR[j] = (previousPR[j]!=0)* (previousPR[j]/g.out_degree[j])  ;
//        }

        #pragma omp parallel for schedule(dynamic,256)
        for (int j = 0; j < g.src_size ; ++j) {
            // too much random access here
            for (int k = g.src[j].second; k < g.src[j+1].second ; ++k) {
                #pragma omp atomic
                PR[dst[k]]+= previousPR[g.src[j].first]/g.out_degree[g.src[j].first] ;
            }
        }

        #pragma omp parallel for
        for (int j = 0; j < n; ++j) {
            PR[j] = d*PR[j] + (1 -d)/(float)n ;
        }

       // swap PR and previousPR
        temp = previousPR ;
        previousPR = PR ;
        PR  =  temp ;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "calculating pageRank took : " << duration.count() << '\n' ;


    std::ofstream out("/home/farouk/CLionProjects/graph-creation/inputs/PageRankOutput.txt");
    for (int j = 0; j < n; ++j) {
        out << previousPR[j]  ;
        out << '\n' ;
    }
    out.close();

    std::sort(previousPR,previousPR+n);

    std::cout << "Top 10 vertices : \n" ;
    for (int i = n-1; i >n-10   ; i--) {
        std::cout << previousPR[i] << '\n' ;
    }
    std::cout << '\n' ;

    std::cout << "sum of al PR = " << std::accumulate(previousPR,previousPR+n,0.0) << '\n';


    delete [] PR ;
    delete [] previousPR ;
}


int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <vertices> <nb_iterations> " << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const int vertices = atoi(argv[2]);
    const int nb_iteration = atoi(argv[3]);

    parallelPageRank(filename,vertices,nb_iteration) ;

    return 0;
}