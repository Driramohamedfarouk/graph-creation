//
// Created by farouk on 31/07/23.
//
#include <iostream>
#include "../graph-creation/ChainedEC.h"
#include "../graph-creation/getDstFile.h"

void OneStepNeighbour(const std::string& path, const int n, const int src){

    auto x = getDstFile(path);
    int *dst = x.first , nb_edges = x.second ;

    ChainedEdgeCentric g = createChainedEdgeCentric(path,n,nb_edges);

    std::cout << "loaded the graph in memory " << '\n' ;

    node_t* head = g.indexing[src].next ;
    while(head){
        for (int i = (head-1)->offset ; i < head->offset ; ++i) {
            std::cout << dst[i] << ' ' ;
        }
        head = head->next ;
    }
    std::cout << '\n' ;

}


int main(int argc, char* argv[]){

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <vertices> <src> " << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const int vertices = atoi(argv[2]);
    const int src = atoi(argv[3]);

    OneStepNeighbour(filename,vertices,src) ;

    return 0 ;
}