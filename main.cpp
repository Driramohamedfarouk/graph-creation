#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>


// TODO : transform to Template method
void print_array(const std::vector<int>& v){
    for(int i : v) {
        std::cout << i << "  " ;
    }
    std::cout<< '\n' ;
}

void createGraphFromFile(const std::string& path){
    std::vector<int> src;
    std::vector<int> count;
    std::vector<int> dst;

    std::ifstream file(path);
    int a,b ;
    file >> a >> b ;
    std::cout << a << " " << b << '\n' ;
    // TODO : consider pushing directly a binary
    // TODO : consider performance consequences of reallocating
    src.push_back(a);
    count.push_back(1);
    dst.push_back(b);
    while (file >> a >> b){
        std::cout << a << " " << b << '\n' ;
        // TODO : store prev variable better than accessing last element of the vector each time
        // TODO : collect metadata of the graph in the pass and store them somewhere
        if ( src.back()!=a )
        {
            src.push_back(a);
            count.push_back(1);
        }
        else
        {
            count.back()++;
        }

        dst.push_back(b);
    }
    std::cout << "source : \n"  ;
    print_array(src) ;

    std::cout << "count : \n"  ;
    print_array(count) ;

    std::cout << "destination : \n"  ;
    print_array(dst) ;

}

void createGraphAndBinaryFromFile(const std::string& path,const std::string& out_path){
    std::vector<int> src;
    std::vector<int> count;

    std::ifstream file(path);

    std::ofstream dst ;
    dst.open(out_path,std::ios::binary|std::ios::out);

    int a,b ;
    file >> a >> b ;
    std::cout << a << " " << b << '\n' ;
    // TODO : consider pushing directly a binary
    // TODO : consider performance consequences of reallocating
    src.push_back(a);
    count.push_back(1);

    dst.write((char *)&b,sizeof(int));
    while (file >> a >> b){
        // std::cout << a << " " << b << '\n' ;
        // TODO : store prev variable better than accessing last element of the vector each time
        // TODO : collect metadata of the graph in the pass and store them somewhere
        if ( src.back()!=a )
        {
            src.push_back(a);
            count.push_back(1);
        }
        else
        {
            count.back()++;
        }

        dst.write((char *)&b,sizeof(int));

    }
    std::cout << "source : \n"  ;
    print_array(src) ;

    std::cout << "count : \n"  ;
    print_array(count) ;

}



void createGraphInParallelFromFile(const std::string& path){
    std::vector<int> src; //vector containing the source of the edge
    std::vector<int> count; // vector containing the number of consecutive edge for a given node
    std::vector<int> dst;

    std::ifstream file(path);
    int a,b ;
    file >> a >> b ;
    std::cout << a << " " << b << '\n' ;
    // TODO : consider pushing directly a binary
    src.push_back(a);
    count.push_back(1);
    dst.push_back(b);
    while (file >> a >> b){
        std::cout << a << " " << b << '\n' ;
        // TODO : store prev variable better than accessing last element of the vector
        // TODO : collect metadata of the graph in the pass and store them somewhere
        if ( src.back()!=a )
        {
            src.push_back(a);
            count.push_back(1);
        }
        else
        {
            count.back()++;
        }

        dst.push_back(b);
    }
    std::cout << "source : \n"  ;
    print_array(src) ;

    std::cout << "count : \n"  ;
    print_array(count) ;

    std::cout << "destination : \n"  ;
    print_array(dst) ;

}


void openAndSeekEnd(const std::string& path){
    std::ifstream file ;
    file.open(path);
    auto start = std::chrono::high_resolution_clock::now();
    file.seekg(10e5,std::ios::beg);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);

    std::cout << duration.count() << '\n' ;
}



int main() {
    createGraphFromFile("/home/farouk/CLionProjects/untitled/el.txt") ;
    //openAndSeekEnd("/home/farouk/CLionProjects/untitled/el.txt") ;
    return 0;
}
