//
// Created by farouk on 15/08/23.
//

// this is an implementation of the state of the art Afforest algorithm [1]
// for calculating connected components of a graph.

// the output of this method conform to the GAP Benchmark suite [2]
// produces an array of 32-bit integer representing the label for
// each connected component.

// if the graph is directed this method returns the weakly connected component.

// [1] Michael Sutton, Tal Ben-Nun, and Amnon Barak. "Optimizing Parallel
//     Graph Connectivity Computation via Subgraph Sampling" Symposium on
//     Parallel and Distributed Processing, IPDPS 2018.


#include <iostream>

#include "../graph-creation/getDstFile.h"
#include "../graph-creation/EC.h"
#include "../utils/timer.h"
#include "../utils/result_writer.h"

void link(int u , int v,int* parent){
    int p1 = parent[u] , p2 = parent[v] , h ,l ;
    while (p1!=p2){
        h = std::max(p1,p2) ;
        l = p1 + p2 - h ;
        if ((parent[h] == l) ||
            (parent[h] == h && __sync_bool_compare_and_swap(&parent[h],h,l)))
            break;
        p1  = parent[parent[h]] ;
        p2 = parent[l] ;
    }
}


void compress(int v, int* parent){
    while (parent[v]!=parent[parent[v]]){
        parent[v] = parent[parent[v]] ;
    }
}

void connected_components(const char *path , int n){

    // construct the EC representation of the graph
    auto [dst,nb_edges] = getDstFile(path);

    ExtendedPairEdgeCentric g = BranchlessCreateGraphFromFilePageRank(path,n,nb_edges);
    std::cout << "loaded the graph in memory " << '\n' ;


    Timer timer ;
    timer.Start() ;

    // initialize parent array
    int* parent = reinterpret_cast<int * >(malloc(n*sizeof(int)));
    //std::atomic<int> parent[n] ;
    #pragma omp parallel for
    for(int i= 0 ; i<n ; ++i){
        parent[i] = i ;
    }


    #pragma omp parallel for schedule(dynamic,512)
    for (int l = 0; l < g.src_size ; ++l) {
        for (int k = g.src[l].second; k < g.src[l + 1].second ; ++k) {
            link(g.src[l].first,dst[k],parent);
        }
    }
    std::cout << "finished linking \n" ;

    #pragma omp parallel for schedule(dynamic,128)
    for(int i= 0 ; i<n ; ++i){
        compress(i, parent) ;
    }
    timer.Stop() ;
    std::cout << "calculating connected components took " << timer.Microsecs() << '\n' ;

    Writer<int>::write("CCOutput.txt",parent,n);

}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <vertices> " << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const int vertices = atoi(argv[2]);

    connected_components(filename,vertices) ;

    return 0;
}