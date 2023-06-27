//
// Created by farouk on 26/06/23.
//
#include <iostream>
#include <vector>
#include <omp.h>
#include <fstream>

struct EdgeCentric{
    std::vector<int> src;
    std::vector<int> count;
    std::vector<int> dst;
};


void print_array(const std::vector<int>& v){
    for(int i : v) {
        std::cout << i << "  " ;
    }
    std::cout<< '\n' ;
}



void convertToBinary(const std::string& path){
    std::ifstream  el(path);
    std::ofstream binary_file(path+".bin", std::ios::out|std::ios::binary);
    int a,b ;

    while (el >> a >> b ){
        binary_file.write((const char *)&a,sizeof(int));
        binary_file.write((const char *)&b,sizeof(int));
    }
}


void parallelGraphCreation(const std::string& path,const int nb_edges) {
    EdgeCentric graphs[8];
    auto step = nb_edges / 8;


#pragma omp parallel
    {
        auto thread_id = omp_get_thread_num();
        std::ifstream edge_list(path);
        edge_list.seekg(thread_id *8 *step, std::ios::beg);
        int a, b, i = 1 ;
        edge_list.read((char *)&a,sizeof(int) );
        edge_list.read((char *)&b,sizeof(int) );
        graphs[thread_id].src.push_back(a);
        graphs[thread_id].count.push_back(1);
        graphs[thread_id].dst.push_back(b);
        while (i < step ) {
            // std::cout << a << " " << b << '\n';
            // TODO : store prev variable better than accessing last element of the vector each time
            // TODO : collect metadata of the graph in the pass and store them somewhere
            edge_list.read((char *)&a,sizeof(int) );
            edge_list.read((char *)&b,sizeof(int) );
            if (graphs[thread_id].src.back() != a) {
                graphs[thread_id].src.push_back(a);
                graphs[thread_id].count.push_back(1);
            } else {
                graphs[thread_id].count.back()++;
            }

            graphs[thread_id].dst.push_back(b);
            i++;
        }
        edge_list.close();
        #pragma omp barrier
        /*
         * merge step : allocate a fixed sized array as the sum of all arrays
         * memcpy() content of each vector to the array and free the memory used by the vector
         * */
        #pragma omp critical
        {
        std::cout << "count " << thread_id <<  ": \n";
        print_array(graphs[thread_id].dst);
        };
}
}
int main() {
    std::cout << "hello " << '\n' ;
    convertToBinary("/home/farouk/CLionProjects/untitled/el.txt");
    parallelGraphCreation("/home/farouk/CLionProjects/untitled/el.txt.bin",24);
}