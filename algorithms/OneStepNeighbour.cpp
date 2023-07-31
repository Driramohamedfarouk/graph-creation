//
// Created by farouk on 31/07/23.
//
#include <iostream>
#include "../graph-creation/ChainedEC.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

void OneStepNeighbour(const std::string& path, const int n, const int src){
    int fd , nb_edges ;
    fd = open((path+".dst.bin").c_str(),O_RDONLY) ;
    if (fd == -1) {
        perror ("open");
        exit(1);
    }

    struct stat sb{};

    if (fstat (fd, &sb) == -1) {
        perror ("fstat");
        exit(1);
    }

    nb_edges = sb.st_size/ sizeof(int) ;

    ChainedEdgeCentric g = createChainedEdgeCentric(path,n,nb_edges);

    int *dst = static_cast<int *>(mmap(nullptr, sb.st_size, PROT_READ, MAP_SHARED, fd, 0));


    if (dst == MAP_FAILED) {
        perror ("mmap");
        exit(1);
    }
    if (close (fd) == -1) {
        perror ("close");
        exit(1);
    }

    std::cout << "loaded the graph in memory " << '\n' ;

    node_t* head = g.indexing[src].next ;
    while(head){
        for (int i = (head-1)->offset ; i < head->offset ; ++i) {
            std::cout << dst[i] << ' ' ;
        }
        head = head->next ;
    }
    std::cout << '\n' ;

}


int main(int argc, char* argv[]){

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <vertices> <src> " << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const int vertices = atoi(argv[2]);
    const int src = atoi(argv[3]);

    OneStepNeighbour(filename,vertices,src) ;

    return 0 ;
}