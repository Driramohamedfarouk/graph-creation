//
// Created by farouk on 17/07/23.
//
#include "bfs.h"

#include <chrono>
#include <fstream>

#include "../graph-creation/getDstFile.h"
#include "../graph-creation/ChainedEC.h"


void bfs(const std::string& path, int src, int n ) {
    int* dst ,nb_edges ;
    auto x = getDstFile(path) ;
    dst = x.first ; nb_edges = x.second ;

    ChainedEdgeCentric g = createChainedEdgeCentric(path, n,nb_edges);
    std::cout << "loaded the graph in memory " << '\n' ;

    auto start = std::chrono::high_resolution_clock::now();
    auto *lvl = new int[n];

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        lvl[i] = -1 ;
    }


    int *degrees = new int[n] ;

    int current_lvl = 0;

    node_t* head ;
    lvl[src] = 0;
    // initialize empty frontier and next frontier
    // TODO : consider better Data Structure that can be splitted efficiently
    int * frontier, *next_frontier;
    frontier = new int[n] ;
    next_frontier = new int[nb_edges] ;
    frontier[0] = src ;
    int frontierSize = 1 , d , sum ;
    while (frontierSize>0) {
        std::cout << "frontier size -> " << frontierSize << '\n' ;
        current_lvl++;
        // construct an array of frontier prefixed degrees
        sum = 0 ;
        for (int i = 0; i < frontierSize ; ++i) {
            degrees[i] = sum ;
            sum+=g.out_degree[frontier[i]] ;
        }
        int prev ;
        #pragma omp parallel for schedule(dynamic,1024) private(prev,d,head)
        for(int i =  0 ; i< frontierSize ;++i) {
            head = g.indexing[frontier[i]].next ;
            d = 0 ;
            while (head){
                for (int j = (head-1)->offset; j < head->offset; j++) {
                    // replace this with CAS and can safely parallelize outer loop
                    //std::cout << dst[j] << " -> " << lvl[dst[j]] << '\n' ;
                    if (lvl[dst[j]]==-1){
                        #pragma omp atomic capture
                        {
                            prev = lvl[dst[j]];
                            lvl[dst[j]] = current_lvl ;
                        }
                        if(prev==-1)
                            next_frontier[degrees[i] +d ] = dst[j] ;
                    } else {
                        next_frontier[degrees[i] + d] = -1;
                    }
                    d++ ;
                }
                head= head->next ;
            }
        }
        // get rid of -1 values ;
        int k = 0 ;
        //std::cout << "sum - > " << sum << '\n' ;
        // total number of degrees
        for (int i = 0; i < sum ; ++i) {
            if(next_frontier[i]!=-1) {
                frontier[k++] = next_frontier[i] ;
            }
        }

        frontierSize = k ;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "calculating bfs took : " << duration.count() << '\n' ;



    // TODO : define a class writer
    std::ofstream out("/home/farouk/CLionProjects/graph-creation/inputs/BFSOutput.txt");
    for (int j = 0; j < n; ++j) {
        out << lvl[j]  ;
        out << '\n' ;
    }
    out.close();

};


int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <src> <vertices> " << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const int src = atoi(argv[2]);
    const int vertices = atoi(argv[3]);

    bfs(filename,src,vertices);

    return 0;
}