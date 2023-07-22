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

void parallelPageRank(const std::string& path, const int n, const int nb_iteration){

    ExtendedEdgeCentric g ;
    g = createGraphFromFilePageRank(path,n);
/*
    for (int i = 0; i < n; ++i) {
        std::cout << *(g.out_degree+i) << " " ;
    }
    std::cout << '\n' ;

    */
    int fd ;

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
            previousPR[j] /= g.out_degree[j];
        }

        // partition the source array
        // the performance of parallel pageRank depends on the partitioning of edges
        // for each source add the value of previous[source] to count[source] next destinations
        // TODO : change the factor to a power of two and compare results
        //std::cout << g.src.size() << '\n' ;
        #pragma omp parallel for schedule(dynamic,10)
        for (int j = 0; j < g.src.size()-2 ; j+=2) {
            // internal loop should not be done in parallel
            // too much random access here
            //std::cout << g.src[j+1] << '\n' ;

            for (int k = g.src[j]; k < g.src[j+2] ; ++k) {
                PR[dst[k]]+=previousPR[g.src[j+1]] ;
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

//    for (int i = 0; i < n; ++i) {
//        std::cout << previousPR[i]  << ' ';
//    }
//    std::cout << '\n' ;

//    std::ofstream out("/home/farouk/CLionProjects/graph-creation/inputs/PageRankOutput.txt");
//    for (int j = 0; j < n; ++j) {
//        out << previousPR[j]  ;
//        out << '\n' ;
//    }
//    out.close();
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