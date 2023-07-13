//
// Created by farouk on 05/07/23.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

void convertToBinary(const std::string& path){
    // TODO : if defined benchmarking remove the first while loop
    auto start = std::chrono::high_resolution_clock::now();
    std::string line ;
    std::ifstream  el(path);
    while(std::getline(el,line) && line[0]=='#');
    std::ofstream binary_file(path+".bin", std::ios::out|std::ios::binary);
    int a,b, nb_edges = 0  ;
    std::istringstream iss(line);
    iss >> a >> b  ;
    binary_file.write((const char *)&a,sizeof(int));
    binary_file.write((const char *)&b,sizeof(int));
    nb_edges++ ;
    while (el >> a >> b ){
        binary_file.write((const char *)&a,sizeof(int));
        binary_file.write((const char *)&b,sizeof(int));
        nb_edges++ ;
    }
    std::ofstream config_file(path+".conf");
    config_file << nb_edges ;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "Conversion to binary took : " << duration.count() << '\n' ;
}

