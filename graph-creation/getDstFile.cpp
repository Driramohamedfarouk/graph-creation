//
// Created by farouk on 03/08/23.
//
#include "getDstFile.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


std::pair<int*,int> getDstFile(const std::string& path ){
    int fd , nb_edges   ;
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

    int *dst = static_cast<int *>(mmap(nullptr, sb.st_size, PROT_READ, MAP_SHARED, fd, 0));


    if (dst == MAP_FAILED) {
        perror ("mmap");
        exit(1);
    }
    if (close (fd) == -1) {
        perror ("close");
        exit(1);
    }
    return  {dst,nb_edges} ;
}