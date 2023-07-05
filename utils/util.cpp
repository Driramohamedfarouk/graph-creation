//
// Created by farouk on 30/06/23.
//
#include <iostream>
#include <vector>
#include "util.h"

// TODO : transform to Template method
template <typename T>
void print_array(const std::vector<T>& v){
    for(T i : v) {
        std::cout << i << "  " ;
    }
    std::cout<< '\n' ;
}


void print_array(const std::vector<int>& v){
    for(int i : v) {
        std::cout << i << "  " ;
    }
    std::cout<< '\n' ;
}

void print_array(const std::vector<double>& v){
    for(double i : v) {
        std::cout << i << "  " ;
    }
    std::cout<< '\n' ;
}