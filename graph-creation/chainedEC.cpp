//
// Created by farouk on 13/07/23.
//
#include <fstream>
#include "chainedEC.h"
#include "../utils/util.h"

// TODO : a use of a data structure that is optimized for accesses last element
//  probably a wrapper over  array that contains pointer to last element

ChainedEdgeCentric createChainedEdgeCentric(const std::string& path,const int n){

    int nb_edges ;
    std::ifstream conf(path+".conf");
    conf >> nb_edges ;

    ChainedEdgeCentric g{} ;
    g.indexing = new int[n] ;
    g.dst = new int[nb_edges] ;

    //g.src.resize(n);

    int* last  = new int[n] ;
    std::fill_n(last, n, -1);

    /*
     * can u initialize last array where the i'st element
     * contain the address of the i'st element of indexing,
     * so you can avoid branches in the code
     * => overcome type mismatch problem
     * */


    std::ifstream edge_list(path+".bin");
    int a,b , prev ;
    edge_list.read((char *)&a,sizeof(int) );
    edge_list.read((char *)&b,sizeof(int) );
    prev = a ;

    g.indexing[a] = 0 ;
    // -1 plays the role of NULL in a linked list
    g.src.emplace_back(-1,1);
    g.dst[0] = b ;

    for (int i = 1; i < nb_edges; ++i) {
        edge_list.read((char *)&a,sizeof(int) );
        edge_list.read((char *)&b,sizeof(int) );
        if ( prev!=a )
        {
            // src vector grows only in this if branch
            // TODO : should consider a data structure that accesses last element directly
            last[prev] = g.src.size()-1;
            prev = a ;
            g.src.emplace_back(-1 ,1);
            if(last[a] == -1){
                g.indexing[a] = last[a] =  g.src.size()-1 ;
            } else {
                // place the current address in the case of last appearance
                g.src[last[a]].first = g.src.size() -1 ;
            }
        }
        else
        {
            g.src.back().second++;
        }
        g.dst[i] = b ;
    }

    delete [] last ;
    printCEC(g,n,nb_edges) ;
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

    std::cout << "destination  : \n"  ;
    for (int i = 0; i < m ; ++i) {
        std::cout << g.dst[i] << " " ;
    }
    std::cout << '\n' ;

}
