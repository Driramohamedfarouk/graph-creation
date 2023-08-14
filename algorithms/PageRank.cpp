//
// Created by farouk on 30/06/23.
//
#include "PageRank.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <immintrin.h>
#include <numeric>
#include <queue>

#include "../graph-creation/EC.h"
#include "../graph-creation/getDstFile.h"
#include "../utils/pv_vector.h"

#define d 0.85f

// prints the biggest 10 elements in the array
// used to print the 10 most important nodes in PageRank
void printTop10(const float *arr,int n);

void parallelPageRank(const std::string& path, int n, const int nb_iteration){


    auto x = getDstFile(path);
    int *dst = x.first ,nb_edges = x.second ;


    ExtendedPairEdgeCentric g = BranchlessCreateGraphFromFilePageRank(path,n,nb_edges);
    std::cout << "loaded the graph in memory " << '\n' ;


    auto start_out_degree = std::chrono::high_resolution_clock::now();


    // TODO : probably precomputing inverses of PR won't improve performance => micro-benchmark
    pv_vector<float> inverse_out_degree(n);
    inverse_out_degree.inverse(g.out_degree);
    _mm_free(g.out_degree) ;

    auto end_out_degree = std::chrono::high_resolution_clock::now();
    auto duration_out_degree = std::chrono::duration_cast<std::chrono::microseconds>(end_out_degree- start_out_degree);
    std::cout << "calculating out degree took : " << duration_out_degree.count() << '\n' ;


    auto start = std::chrono::high_resolution_clock::now();



    pv_vector<float> previousPR(n);
    pv_vector<float> PR(n);

    const float y = 1.0f/(float )n ;
    const float z = y*(1-d) ;

    previousPR.fill(y);

    for (int i = 0; i < nb_iteration; ++i) {

        PR.fill(0.0f);

        previousPR *= inverse_out_degree ;


        float sourcePR ;
        // TODO : microbenchmark using pair or vector of double size
        //  try use hyper object
        #pragma omp parallel for schedule(dynamic,256) private(sourcePR)
        for (int l = 0; l < g.src_size ; ++l) {
            // too much random access here
            sourcePR = previousPR[g.src[l].first] ;
            for (int k = g.src[l].second; k < g.src[l + 1].second ; ++k) {
                #pragma omp atomic
                PR[dst[k]]+= sourcePR ;
            }
        }

        PR.mul_add(d,z);

        pv_vector<float>::swap(PR,previousPR);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "calculating pageRank took : " << duration.count() << '\n' ;

    // TODO : define a writer class
    std::ofstream out("/home/farouk/CLionProjects/graph-creation/inputs/PageRankOutput.txt");
    for (int j = 0; j < n; ++j) {
        out << previousPR[j]  ;
        out << '\n' ;
    }
    out.close();

    printTop10(previousPR.data(),n);

}

void printTop10(const float *arr,int n){
    std::priority_queue <float, std::vector<float>, std::greater<> > pq;
    for(int i= 0 ;i < 10 ;++i){
        pq.push(arr[i]);
    }
    for(int i= 10;i<n ;i++){
        if(arr[i] > pq.top()){
            pq.pop();pq.push(arr[i]);
        }
    }
    std::cout << "Top 10 vertices : \n" ;
    float top[10] ;
    for(int i= 0 ;i < 10 ;++i){
        top[9-i] = pq.top() ;
        pq.pop();
    }
    for(float i : top){
        std::cout << i << '\n' ;
    }

    std::cout << '\n' ;

    std::cout << "sum of al PR = " << std::accumulate(arr, arr + n, 0.0) << '\n';

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