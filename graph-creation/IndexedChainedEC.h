//
// Created by farouk on 13/07/23.
//

#ifndef GRAPH_PROCESSING_FRAMEWORK_CHAINEDEC_H
#define GRAPH_PROCESSING_FRAMEWORK_CHAINEDEC_H

#include <iostream>
#include <vector>

//struct ChainedEdgeCentric{
//    std::pair<int*,int>** indexing;
//    std::vector<std::pair<int*,int>> src;
//    int* dst ;
//};


struct ChainedEdgeCentric{
    int* indexing;
    std::vector<std::pair<int,int>> src;
    int* dst ;
};

ChainedEdgeCentric createChainedEdgeCentric(const std::string& path,int n);

void printCEC(const ChainedEdgeCentric& g,const int n, const int m ) ;


#endif //GRAPH_PROCESSING_FRAMEWORK_CHAINEDEC_H
