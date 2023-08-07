//
// Created by farouk on 30/06/23.
//

#ifndef UNTITLED_EC_H
#define UNTITLED_EC_H
#include <iostream>
#include <vector>

// add nb of edges and vertices to the struct

struct ExtendedPairEdgeCentric{
    std::pair<int,int>* src;
    int* out_degree;
    int src_size;
};

ExtendedPairEdgeCentric BranchlessCreateGraphFromFilePageRank(const std::string& path,const int n,const int nb_edges);



#endif //UNTITLED_EC_H
