//
// Created by farouk on 13/07/23.
//
#include <fstream>
#include <chrono>
#include "chainedEC.h"

// TODO : a use of a data structure that is optimized for accesses last element
//  probably a wrapper over  array that contains pointer to last element

ChainedEdgeCentric createChainedEdgeCentric(const std::string& path,const int n, const int nb_edges){
    auto start = std::chrono::high_resolution_clock::now();
    int src_size ;
    std::ifstream conf(path+".conf");
    conf >> src_size ;

    ChainedEdgeCentric g{} ;
    g.indexing = new int[n] ;

    //g.src.resize(n);

    int* last  = new int[n] ;
    std::fill_n(last, n, -1);
    std::fill_n(g.indexing, n, -1);


    /*
     * can u initialize last array where the i'st element
     * contain the address of the i'st element of indexing,
     * so you can avoid branches in the code
     * => overcome type mismatch problem
     * */


    std::ifstream edge_list(path+".bin");
    int a , prev ;
    edge_list.read((char *)&a,sizeof(int) );
    prev = a ;

    g.indexing[a] = 0 ;
    // -1 plays the role of NULL in a linked list
    g.src.emplace_back(-1,0);
    g.src.emplace_back(-1,1);

    for (int i = 1; i < nb_edges; ++i) {
        edge_list.read((char *)&a,sizeof(int) );
        if ( prev!=a )
        {
            // src vector grows only in this if branch
            // TODO : should consider a data structure that accesses last element directly
            last[prev] = g.src.size()-2;
            prev = a ;
            g.src.emplace_back(-1 ,g.src.back().second);
            if(last[a] == -1){
                g.indexing[a] = last[a] =  g.src.size()-2 ;
            } else {
                // place the current address in the case of last appearance
                g.src[last[a]].first = g.src.size() -2 ;
            }
        }
        g.src.back().second++;
    }
    g.src.emplace_back(-1 ,g.src.back().second+1);
    g.src.shrink_to_fit();
    delete [] last ;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "creating CEC in memory took : " << duration.count() << '\n' ;
    // printCEC(g,n,nb_edges) ;
    std::cout << g.src.size() << '\n' ;
    return g ;
}



void printCEC(const ChainedEdgeCentric& g,const int n, const int m ) {
    std::cout << "indexing : \n"  ;
    for (int i = 0; i < n ; ++i) {
        std::cout << g.indexing[i] << " " ;
    }
    std::cout << '\n' ;

    std::cout << "src : \n"  ;
    for (auto el : g.src) {
        std::cout << "(" << el.first << "," << el.second << ") " ;
    }
    std::cout << '\n' ;
}
