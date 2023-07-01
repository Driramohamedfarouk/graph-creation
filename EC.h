//
// Created by farouk on 30/06/23.
//

#ifndef UNTITLED_EC_H
#define UNTITLED_EC_H
#include <iostream>
#include <vector>
#include "util.h"

// add nb of edges and vertices to the struct
// change values to &
struct EdgeCentric{
    std::vector<int> src;
    std::vector<int> count;
    std::vector<int> dst;
};


struct ExtendedEdgeCentric{
    std::vector<int> src;
    std::vector<int> count;
    std::vector<int> dst;
    std::vector<int> out_degree;
};




// sequential graph creation
EdgeCentric createGraphFromFile(const std::string& path);

ExtendedEdgeCentric createGraphFromFilePageRank(const std::string& path,const int n);

void print_EC(EdgeCentric g);

void print_EEC(ExtendedEdgeCentric& g);


#endif //UNTITLED_EC_H
