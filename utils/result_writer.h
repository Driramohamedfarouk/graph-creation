//
// Created by farouk on 15/08/23.
//

#ifndef GRAPH_PROCESSING_FRAMEWORK_WRITER_H
#define GRAPH_PROCESSING_FRAMEWORK_WRITER_H

#include <iostream>
#include <fstream>

// This class takes an array of type T
// and a file and stores the array to the file.
template<typename T>
class Writer{
private:
    const static std::string directory ;
public:
    static void write(const std::string& kernel,T* array,int n){
        std::ofstream out(directory+kernel);
        for (int j = 0; j < n; ++j) {
            out << array[j]  ;
            out << '\n' ;
        }
        out.close();
    }
};

template <typename T>
const std::string Writer<T>::directory = "/home/farouk/CLionProjects/graph-creation/outputs/";

#endif //GRAPH_PROCESSING_FRAMEWORK_WRITER_H
