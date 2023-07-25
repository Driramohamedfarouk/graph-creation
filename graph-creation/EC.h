//
// Created by farouk on 30/06/23.
//

#ifndef UNTITLED_EC_H
#define UNTITLED_EC_H
#include <iostream>
#include <vector>

// add nb of edges and vertices to the struct
// change values to &
struct EdgeCentric{
    std::vector<int> src;
    std::vector<int> count;
    std::vector<int> dst;
};

/*
 * this is an extended form of EdgeCentric specific for page rank
 * the count become accumulated offset
 * and out degree is calculated
 * */
struct ExtendedEdgeCentric{
    std::vector<int> src;
    int* out_degree;
};

struct ExtendedPairEdgeCentric{
    std::pair<int,int>* src;
    int* out_degree;
    int src_size;
};




// sequential graph creation
EdgeCentric createGraphFromFile(const std::string& path);

ExtendedEdgeCentric createGraphFromFilePageRank(const std::string& path,const int n);
//ExtendedEdgeCentric BranchlessCreateGraphFromFilePageRank(const std::string& path,const int n,const int nb_edges);

ExtendedPairEdgeCentric BranchlessCreateGraphFromFilePageRank(const std::string& path,const int n,const int nb_edges);


EdgeCentric createGraphFromFileBFS(const std::string& path,const int n);


EdgeCentric parallelGraphCreation(const std::string& path);

void print_EC(EdgeCentric g);

void print_EEC(ExtendedEdgeCentric& g);


#endif //UNTITLED_EC_H
