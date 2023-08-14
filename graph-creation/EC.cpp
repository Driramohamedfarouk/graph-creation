//
// Created by farouk on 30/06/23.
//
#include "EC.h"

#include <fstream>
#include <sstream>
#include <immintrin.h>

#include "../utils/timer.h"

ExtendedPairEdgeCentric BranchlessCreateGraphFromFilePageRank(const std::string& path,const int n,const int nb_edges){

    Timer timer ;
    timer.Start();

    ExtendedPairEdgeCentric g{} ;
    //g.out_degree = new int[n]{0};
    // TODO : transform this to a pv_vector<int>
    g.out_degree = static_cast<int*>(_mm_malloc(n * sizeof(int), 32));
    #pragma omp parallel for schedule (static,256)
    for (int i = 0; i < n ; ++i) {
        g.out_degree[i]= 0 ;
    }

    std::ifstream edge_list(path+".src.bin");

    std::ifstream conf(path+".conf");
    conf >> g.src_size ;

    g.src = new std::pair<int,int>[g.src_size] ;
    int a , prev ;
    edge_list.read((char *)&a,sizeof(int) );
    g.src[0] = {a,0} ;
    g.src[1] = {a,1} ;

    prev = a ;
    g.out_degree[a]++;

    int currentIdx = 1 ;

    for (int i = 1; i < nb_edges; i++) {
        edge_list.read((char *)&a,sizeof(int) );
        g.out_degree[a]++;
        g.src[currentIdx] = {a,i};
        currentIdx += prev != a ;
        prev = a ;
    }
    g.src[g.src_size-1] = {a,nb_edges};

    timer.Stop();
    std::cout << "creating EC took : "  << timer.Microsecs() << '\n' ;

//    std::ofstream out("/home/farouk/CLionProjects/graph-creation/inputs/OutDegrees.txt");
//    for (int j = 0; j < n; ++j) {
//        out << g.out_degree[j]  ;
//        out << '\n' ;
//    }
//    out.close();

    return g ;
}
