//
// Created by farouk on 30/06/23.
//
#include <iostream>
#include "../graph-creation/EC.h"
#include "PageRank.h"
#include <cstring>
#include <chrono>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <algorithm>
#include <fstream>
#include <numeric>

void parallelPageRank(const std::string& path, const int n, const int nb_iteration){


    int fd , nb_edges   ;

    fd = open((path+".dst.bin").c_str(),O_RDONLY) ;
    if (fd == -1) {
        perror ("open");
        exit(1);
    }

    struct stat sb{};

    if (fstat (fd, &sb) == -1) {
        perror ("fstat");
        exit(1);
    }

    nb_edges = sb.st_size/ sizeof(int) ;

    ExtendedPairEdgeCentric g = BranchlessCreateGraphFromFilePageRank(path,n,nb_edges);

    int *dst = static_cast<int *>(mmap(nullptr, sb.st_size, PROT_READ, MAP_SHARED, fd, 0));


    if (dst == MAP_FAILED) {
        perror ("mmap");
        exit(1);
    }
    if (close (fd) == -1) {
        perror ("close");
        exit(1);
    }

    // allocate two arrays of size n
    // one contains PR for previous iteration and one contains PR for current iteration
    std::cout << "loaded the graph in memory " << '\n' ;

    auto start = std::chrono::high_resolution_clock::now();


    auto* previousPR = static_cast<double *>(malloc(n * sizeof(double)));
    auto* PR = static_cast<double *>(malloc(n * sizeof(double)));
    double* temp ;

    // damping factor of pagerank
    const float d =0.85;

    memset(previousPR,1.0f,n*sizeof(double ));

    for (int i = 0; i < nb_iteration; ++i) {
        memset(PR, 0, n*sizeof(double ));

        // in a fully parallel manner update previous to previous[i] /= out_degree[i]
        #pragma omp parallel for
        for (int j = 0; j < n; ++j) {
            // vertices with outrigger zero wel not be present in the EC
            // no risk of division by zero
            previousPR[j] = ( previousPR[j] / g.out_degree[j] ) ;
        }

        // partition the source array
        // the performance of parallel pageRank depends on the partitioning of edges
        // for each source add the value of previous[source] to count[source] next destinations
        // TODO : change the factor to a power of two and compare results
        //std::cout << g.src.size() << '\n' ;
        // TODO : control parallel read and write to PR
        #pragma omp parallel for schedule(dynamic,16)
        for (int j = 0; j < g.src_size-1 ; ++j) {
            // internal loop should not be done in parallel
            // too much random access here
            for (int k = g.src[j].second; k < g.src[j+1].second ; ++k) {
                #pragma omp atomic
                PR[dst[k]]+=previousPR[g.src[j].first] ;
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
//
//    for (int i = 0; i < n; ++i) {
//        std::cout << previousPR[i]  << ' ';
//    }
//    std::cout << '\n' ;

    std::ofstream out("/home/farouk/CLionProjects/graph-creation/inputs/PageRankOutput.txt");
    for (int j = 0; j < n; ++j) {
        out << previousPR[j]  ;
        out << '\n' ;
    }
    out.close();

    std::sort(previousPR,previousPR+n);

    for (int i = n; i >n-10   ; i--) {
        std::cout << previousPR[i] << ' ' ;
    }
    std::cout << '\n' ;

    std::cout << std::accumulate(previousPR,previousPR+n,0.0) << '\n';


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