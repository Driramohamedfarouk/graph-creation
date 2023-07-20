//
// Created by farouk on 20/07/23.
//
#include <iostream>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>


void SplitConvertToBinary(const std::string& path){
    // TODO : if defined benchmarking remove the first while loop
    auto start = std::chrono::high_resolution_clock::now();
    std::string line ;
    std::ifstream  el(path);
    while(std::getline(el,line) && line[0]=='#');
    std::ofstream src_file(path + ".src.bin", std::ios::out | std::ios::binary);
    std::ofstream dst_file(path + ".dst.bin", std::ios::out | std::ios::binary);
    int a,b,nb_edges = 1  ;
    std::istringstream iss(line);
    iss >> a >> b  ;
    src_file.write((const char *)&a, sizeof(int));
    dst_file.write((const char *)&b, sizeof(int));
    while (el >> a >> b ){
        src_file.write((const char *)&a, sizeof(int));
        dst_file.write((const char *)&b, sizeof(int));
        nb_edges++ ;
    }
    std::ofstream config_file(path+".conf");
    config_file << nb_edges ;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "Conversion to binary took : " << duration.count() << '\n' ;
}