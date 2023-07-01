//
// Created by farouk on 30/06/23.
//
#include "EC.h"
#include <fstream>
#include "util.h"


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
    std::ifstream file(path);
    std::string line ;
    std::getline(file,line);
    std::cout << line << std::endl ;
    std::getline(file,line);
    std::cout << line << std::endl ;
    std::getline(file,line);
    std::cout << line << std::endl ;
    std::getline(file,line);
    std::cout << line << std::endl ;
    int a,b ;
    file >> a >> b ;
    // TODO : consider pushing directly a binary
    // TODO : consider performance consequences of reallocating
    g.src.push_back(a);
    g.count.push_back(0);
    g.count.push_back(1);
    g.dst.push_back(b);
    g.out_degree[a]++;
    while (file >> a >> b){
        // std::cout << a << " " << b << '\n' ;
        // TODO : store prev variable better than accessing last element of the vector each time
        // TODO : collect metadata of the graph in the pass and store them somewhere
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
