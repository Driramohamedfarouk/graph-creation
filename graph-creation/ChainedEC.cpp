//
// Created by farouk on 19/07/23.
//

#include "ChainedEC.h"
#include "chrono"
#include <fstream>
#include <cassert>

ChainedEdgeCentric createChainedEdgeCentric(const std::string& path, const int n, const int nb_edges){
    auto start = std::chrono::high_resolution_clock::now();
    int src_size , currentIdx = 0 , count= 1 ;

    std::ifstream conf(path+".conf");
    conf >> src_size ;

    std::cout << src_size << '\n' ;
    src_size-- ;
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

    g.indexing[a].next =last[a] = g.src ;
    g.src[0] = node_t{1, nullptr} ;
    //assert( *(last[0]) == g.indexing[0] ) ;
    // g.src[1] = node_t{1, nullptr} ;
    for (int i = 1; i < nb_edges; ++i) {
        edge_list.read((char *)&a,sizeof(int) );
        g.src[currentIdx] = node_t{count, nullptr} ;
        //std::cout << currentIdx << "->" << count << '\n';
        count *=  prev == a  ;
        count++ ;
        currentIdx += prev != a;
        prev = a ;
        last[a]  = (*last[a]).next = &g.src[currentIdx];
    }
    g.src[currentIdx]= node_t{count, nullptr};
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
//        std::cout <<'(' << g.src[i].count << ','<< g.src[i].next << ") " ;
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
    std::cout << "creating CEC in memory took : " << duration.count() << '\n' ;
    //std::cout << currentIdx << '\n' ;
    return g ;
}


void traverse(const std::string& path, const int n, const int nb_edges, const int src){
    ChainedEdgeCentric g = createChainedEdgeCentric(path,n , nb_edges) ;
    node_t* head = g.indexing[src].next ;
    while(head){
        std::cout << head->count << ' ' ;
        head = head->next ;
    }
    std::cout << '\n' ;

}


int main(int argc, char* argv[]) {

    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <filename> <vertices> <nb_edges> " << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const int vertices = atoi(argv[2]);
    const int nb_edges = atoi(argv[3]);
    const int src = atoi(argv[4]);

    //createChainedEdgeCentric(filename,vertices,nb_edges) ;
    traverse(filename, vertices, nb_edges, src) ;
    //traverse("../inputs/soc-LiveJournal1.txt", 4847571, 68993773, 3) ;
    return 0;
}