//
// Created by farouk on 19/07/23.
//
#include "ChainedEC.h"

#include <fstream>

#include "../../utils/timer.h"

ChainedEdgeCentric createChainedEdgeCentric(const std::string& path,int n,int nb_edges){
    Timer timer ;
    timer.Start();

    ChainedEdgeCentric g{} ;

    g.out_degree = new int[n]{0};
    g.indexing = reinterpret_cast<node_t*>(malloc(n* sizeof(node_t ))) ;
    g.src = reinterpret_cast<node_t*>(malloc(nb_edges* sizeof(node_t ))) ;

    auto** last  = reinterpret_cast<node_t**>(malloc(n* sizeof(node_t *))) ;

    std::fill_n(g.indexing, n, node_t{-1, nullptr});

    node_t* temp = g.indexing ;
    for (int i = 0; i < n; ++i) {
        last[i] = temp++;
    }

    std::ifstream edge_list(path+".bin");
    int a,b;
    for (int i = 0; i < nb_edges; ++i) {
        edge_list.read((char *)&a,sizeof(int) );
        edge_list.read((char *)&b,sizeof(int)) ;
        g.out_degree[a]++ ;
        g.src[i] = node_t{b, nullptr} ;
        last[a]  = (*last[a]).next = &g.src[i];
    }

    free(last) ;
    timer.Stop();
    std::cout << "creating Chained CEC in memory took : " << timer.Microsecs() << '\n' ;
    return g ;
}
