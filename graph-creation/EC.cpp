//
// Created by farouk on 30/06/23.
//
#include "EC.h"
#include <fstream>
#include <chrono>
#include <sstream>
#include "../utils/util.h"


EdgeCentric createGraphFromFile(const std::string& path){
    EdgeCentric g ;
    std::ifstream file(path);

    std::string line ;
    while(std::getline(file,line) && line[0]=='#');
    int a,b ;
    std::istringstream iss(line);
    iss >> a >> b  ;
    std::cout << a << " " << b << '\n' ;
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

/*
 * Conversion to binary should be done before calling this method.
 * The method creates an extended version of EC that contains
 * out_degree edges, witch is suited for computing PageRank.
 * */
ExtendedEdgeCentric createGraphFromFilePageRank(const std::string& path,const int n){
    auto start = std::chrono::high_resolution_clock::now();
    ExtendedEdgeCentric g ;
    g.out_degree.resize(n,0);
    std::ifstream edge_list(path+".bin");
    int a,b , prev ;
    edge_list.read((char *)&a,sizeof(int) );
    edge_list.read((char *)&b,sizeof(int) );
    // TODO : consider performance consequences of reallocating
    g.src.resize(n);
    g.count.resize(n);
    g.src.push_back(a);
    prev = a ;
    g.count.push_back(0);
    g.count.push_back(1);
    g.out_degree[a]++;

    long nb_edges ;
    std::ifstream conf(path+".conf");
    conf >> nb_edges ;
    g.dst.resize(nb_edges);
    g.dst[0] = b ;
    for (int i = 1; i < nb_edges; ++i) {
        // TODO : collect metadata of the graph in the pass and store them somewhere
        edge_list.read((char *)&a,sizeof(int) );
        edge_list.read((char *)&b,sizeof(int) );
        g.out_degree[a]++;
        if ( prev!=a )
        {
            prev = a ; 
            g.src.push_back(a);
            g.count.push_back(g.count.back()+1);
        }
        else
        {
            g.count.back()++;
        }

        g.dst[i] = b ;
    }
    g.count.push_back(g.count.back()+1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "creating EC took : "  << duration.count() << '\n' ;
    return g ;
}



EdgeCentric createGraphFromFileBFS(const std::string& path,const int n){
    auto start = std::chrono::high_resolution_clock::now();
    EdgeCentric g ;
    std::ifstream edge_list(path+".bin");
    int a,b , prev ;
    edge_list.read((char *)&a,sizeof(int) );
    edge_list.read((char *)&b,sizeof(int) );
    // TODO : consider performance consequences of reallocating
    g.src.reserve(n);
    g.count.reserve(n);
    g.src.push_back(a);
    prev = a ;
    g.count.push_back(0);
    g.count.push_back(1);

    long nb_edges ;
    std::ifstream conf(path+".conf");
    conf >> nb_edges ;
    g.dst.resize(nb_edges);
    g.dst[0] = b ;
    for (int i = 1; i < nb_edges; ++i) {
        // TODO : collect metadata of the graph in the pass and store them somewhere
        edge_list.read((char *)&a,sizeof(int) );
        edge_list.read((char *)&b,sizeof(int) );
        if ( prev!=a )
        {
            prev = a ;
            g.src.push_back(a);
            g.count.push_back(g.count.back()+1);
        }
        else
        {
            g.count.back()++;
        }

        g.dst[i] = b ;
    }
    g.count.push_back(g.count.back()+1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "creating EC took : "  << duration.count() << '\n' ;
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
    print_array(g.src) ;

    std::cout << "count : \n"  ;
    print_array(g.count) ;

    std::cout << "destination : \n"  ;
    print_array(g.dst) ;

    std::cout << "out degree : \n"  ;
    print_array(g.out_degree) ;
}
