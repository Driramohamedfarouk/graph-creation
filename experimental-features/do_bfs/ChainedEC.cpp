//
// Created by farouk on 19/07/23.
//
#include "ChainedEC.h"

#include <fstream>

#include "../../utils/timer.h"

ChainedEdgeCentric createChainedEdgeCentric(const std::string& path,int n,int m){
    Timer timer ;
    timer.Start();

    ChainedEdgeCentric g{} ;

    g.vertices = n ;
    g.edges = m ;
    g.out_degree = new int[n]{0};
    g.head = reinterpret_cast<node_t*>(malloc(n* sizeof(node_t ))) ;
    g.head_i = reinterpret_cast<node_t*>(malloc(n* sizeof(node_t ))) ;
    g.src = reinterpret_cast<node_t*>(malloc(m* sizeof(node_t ))) ;
    g.src_i = reinterpret_cast<node_t*>(malloc(m* sizeof(node_t ))) ;

    auto** last  = reinterpret_cast<node_t**>(malloc(n* sizeof(node_t *))) ;
    auto** last_i  = reinterpret_cast<node_t**>(malloc(n* sizeof(node_t *))) ;

    std::fill_n(g.head, n, node_t{-1, nullptr});
    std::fill_n(g.head_i, n, node_t{-1, nullptr});

    node_t* temp = g.head ;
    node_t* temp_i = g.head_i ;
    for (int i = 0; i < n; ++i) {
        last[i] = temp++;
        last_i[i]  = temp_i++;
    }

    std::ifstream edge_list(path+".bin");
    int a,b;
    for (int i = 0; i < g.edges; ++i) {
        edge_list.read((char *)&a,sizeof(int)) ;
        edge_list.read((char *)&b,sizeof(int)) ;
        g.out_degree[a]++ ;
        g.src[i] = node_t{b, nullptr} ;
        last[a]  = (*last[a]).next = &g.src[i];
        g.src_i[i] = node_t{a, nullptr} ;
        last_i[b]  = (*last_i[b]).next = &g.src_i[i];
    }

    free(last) ;
    free(last_i) ;
    timer.Stop();
    std::cout << "creating Chained CEC in memory took : " << timer.Microsecs() << '\n' ;
    return g ;
}
