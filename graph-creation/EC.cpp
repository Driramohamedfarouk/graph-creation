//
// Created by farouk on 30/06/23.
//
#include "EC.h"
#include <fstream>
#include "../utils/util.h"


EdgeCentric createGraphFromFile(const std::string& path){
    EdgeCentric g ;
    std::ifstream file(path);
    int a,b ;
    file >> a >> b ;
    std::cout << a << " " << b << '\n' ;
    // TODO : consider pushing directly a binary
    // TODO : consider performance consequences of reallocating
    g.src.push_back(a);
    g.count.push_back(1);
    g.dst.push_back(b);
    while (file >> a >> b){
        // std::cout << a << " " << b << '\n' ;
        // TODO : store prev variable better than accessing last element of the vector each time
        // TODO : collect metadata of the graph in the pass and store them somewhere
        if ( g.src.back()!=a )
        {
            g.src.push_back(a);
            g.count.push_back(1);
        }
        else
        {
            g.count.back()++;
        }

        g.dst.push_back(b);
    }
    return g ;
}


ExtendedEdgeCentric createGraphFromFilePageRank(const std::string& path,const int n){
    ExtendedEdgeCentric g ;
    g.out_degree.resize(n,0);
    std::ifstream edge_list(path+".bin");
    int a,b ;
    edge_list.read((char *)&a,sizeof(int) );
    edge_list.read((char *)&b,sizeof(int) );
    // TODO : consider pushing directly a binary
    // TODO : consider performance consequences of reallocating
    g.src.push_back(a);
    g.count.push_back(0);
    g.count.push_back(1);
    g.dst.push_back(b);
    g.out_degree[a]++;

    long nb_edges ;
    std::ifstream conf(path+".conf");
    conf >> nb_edges ;
    for (int i = 1; i < nb_edges; ++i) {
        // TODO : store prev variable better than accessing last element of the vector each time
        // TODO : collect metadata of the graph in the pass and store them somewhere
        edge_list.read((char *)&a,sizeof(int) );
        edge_list.read((char *)&b,sizeof(int) );
        g.out_degree[a]++;
        if ( g.src.back()!=a )
        {
            g.src.push_back(a);
            g.count.push_back(g.count.back()+1);
        }
        else
        {
            g.count.back()++;
        }

        g.dst.push_back(b);
    }
    g.count.push_back(g.count.back()+1);
    return g ;
}

void print_EC(EdgeCentric g){
    std::cout << "source : \n"  ;
    print_array(g.src) ;

    std::cout << "count : \n"  ;
    print_array(g.count) ;

    std::cout << "destination : \n"  ;
    print_array(g.dst) ;
}

void print_EEC(ExtendedEdgeCentric& g){
    std::cout << "source : \n"  ;
    //print_array(g.src) ;

    std::cout << "count : \n"  ;
    //print_array(g.count) ;

    std::cout << "destination : \n"  ;
    //print_array(g.dst) ;

    std::cout << "out degree : \n"  ;
    //print_array(g.out_degree) ;
}
