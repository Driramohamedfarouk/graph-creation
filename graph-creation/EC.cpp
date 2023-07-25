//
// Created by farouk on 30/06/23.
//
#include "EC.h"
#include <fstream>
#include <chrono>
#include <sstream>


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
    g.out_degree = new int[n]{0};
    std::ifstream edge_list(path+".src.bin");
    // TODO : get number edges from edge_list size
    int nb_edges ;
    std::ifstream conf(path+".conf");
    conf >> nb_edges ;

    int a , prev ;
    edge_list.read((char *)&a,sizeof(int) );
    // TODO : consider performance consequences of reallocating
    g.src.reserve(2*n+2);
    g.src.push_back(0) ;
    g.src.push_back(a) ;
    g.src.push_back(1) ;
    prev = a ;
    g.out_degree[a]++;
    for (int i = 1; i < nb_edges; ++i) {
        // TODO : collect metadata of the graph in the pass and store them somewhere
        edge_list.read((char *)&a,sizeof(int) );
        g.out_degree[a]++;
        // TODO : is there a way to tell the compiler to leave prev in the register during the whole loop ?
        if ( prev!=a )
        {
            g.src.push_back(a);
            g.src.push_back(g.src[g.src.size()-2]);
            prev = a ;
        }
        g.src.back()++;
    }
    //g.src.push_back(g.count.back()+1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "creating EC took : "  << duration.count() << '\n' ;
    //print_array(g.src);
    /*for (auto & i : g.src) {
        std::cout << "(" << i.first << "," << i.second << ") " ;
    }
    std::cout << '\n' ;*/
    std::cout << " src size = " << g.src.size()/2 << "\n" ;
    return g ;
}




ExtendedPairEdgeCentric BranchlessCreateGraphFromFilePageRank(const std::string& path,const int n,const int nb_edges){

    auto start = std::chrono::high_resolution_clock::now();
    ExtendedPairEdgeCentric g{} ;
    g.out_degree = new int[n]{0};

    std::ifstream edge_list(path+".src.bin");

    std::ifstream conf(path+".conf");
    conf >> g.src_size ;

    g.src = new std::pair<int,int>[g.src_size] ;
    int a , prev ;
    edge_list.read((char *)&a,sizeof(int) );
    g.src[0] = {a,0} ;
    g.src[1] = {a,1} ;

    prev = a ;
    g.out_degree[a]++;

    int currentIdx = 1 ;

    for (int i = 1; i < nb_edges; i++) {
        edge_list.read((char *)&a,sizeof(int) );
        g.out_degree[a]++;
        g.src[currentIdx] = {a,i};
        currentIdx += prev != a;
        prev = a ;
    }
    g.src[g.src_size-1] = {a,nb_edges};



    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "creating EC took : "  << duration.count() << '\n' ;

    /* for (int i = 0 ; i< g.src_size ; i++) {
        std::cout << "(" << g.src[i].first << "," << g.src[i].second << ") " ;
    }*/
    //exit(0) ;

    std::cout << '\n' ;
    std::cout << "src size = " << g.src_size << "\n" ;
    std::cout << "nb edges = " << nb_edges << "\n" ;

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
