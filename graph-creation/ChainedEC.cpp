//
// Created by farouk on 19/07/23.
//

#include "ChainedEC.h"
#include "chrono"
#include <fstream>
#include <cassert>

ChainedEdgeCentric createChainedEdgeCentric(const std::string& path, const int n, const int nb_edges){
    auto start = std::chrono::high_resolution_clock::now();
    int src_size , currentIdx = 1 ;

    std::ifstream conf(path+".conf");
    conf >> src_size ;

    std::cout << src_size << '\n' ;
    ChainedEdgeCentric g{} ;
    g.indexing = (node_t*)  malloc(n* sizeof(node_t )) ;
    g.src = (node_t*)  malloc(src_size* sizeof(node_t )) ;

    auto** last  = (node_t**)  malloc(n* sizeof(node_t *)) ;
    std::fill_n(g.indexing, n, node_t{-1, nullptr});
    //std::fill_n(last, n, nullptr);
    node_t* temp = g.indexing ;
    for (int i = 0; i < n; ++i) {
        last[i] = temp++;
    }

//    std::cout << "last : \n" ;
//    for (int i = 0; i < n; ++i) {
//        std::cout << last[i] << ' ' ;
//    }
//    std::cout << '\n' ;

    std::ifstream edge_list(path+".src.bin");
    int a, prev ;
    edge_list.read((char *)&a,sizeof(int) );
    prev = a ;

    g.indexing[a].next =last[a] = g.src+1 ;
    g.src[0] = node_t{0, nullptr} ;
    //assert( *(last[0]) == g.indexing[0] ) ;
    //g.src[1] = node_t{1, nullptr} ;
    for (int i = 1; i < nb_edges; ++i) {
        edge_list.read((char *)&a,sizeof(int) );
        g.src[currentIdx] = node_t{i, nullptr} ;
        currentIdx += prev != a;
        prev = a ;
        last[a]  = (*last[a]).next = &g.src[currentIdx];
    }
    g.src[currentIdx]= node_t{nb_edges, nullptr};
//    assert(currentIdx==src_size);
//    std::cout << "\naddresses of src \n" ;
//    for (int i = 0; i < src_size; ++i) {
//        std::cout << g.src+i << ' ' ;
//    }
//    std::cout << '\n' ;
//    std::cout << "last : \n" ;
//    for (int i = 0; i < n; ++i) {
//        std::cout << last[i] << ' ' ;
//    }
//    std::cout << '\n' ;
//    std::cout << "g.src : \n" ;
//    for (int i = 0; i < src_size; ++i) {
//        std::cout <<'(' << g.src[i].offset << ','<< g.src[i].next << ") " ;
//    }
//    std::cout << '\n' ;
//    std::cout << "g.indexing.next : \n" ;
//    for (int i = 0; i < n; ++i) {
//        std::cout << g.indexing[i].next << ' ' ;
//    }
//    std::cout << '\n' ;

    delete [] last ;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "creating Chained CEC in memory took : " << duration.count() << '\n' ;
    //std::cout << currentIdx << '\n' ;
    return g ;
}
