//
// Created by farouk on 17/07/23.
//
#include <chrono>
#include "../graph-creation/chainedEC.h"
#include "bfs.h"

void bfs(const std::string& path, int src, int n ) {
    ChainedEdgeCentric g = createChainedEdgeCentric(path, n);
    std::cout << "loaded the graph in memory " << '\n' ;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> lvl;
    lvl.resize(n, -1);
    int current_lvl = 0;
    lvl[src] = 0;
    // initialize empty frontier and next frontier
    // TODO : consider better Data Structure that can be splitted efficiently
    std::vector<int> frontier, next_frontier;
    frontier.push_back(src);
    while (!frontier.empty()) {
        // std::cout << frontier.size() << '\n' ;
        for (int v : frontier) {
            // go through all neighbours
            int start = g.indexing[v] ;
            while (start!=-1){
                for (int i = g.src[start].second; i <= g.src[start + 1].second; i++) {
                    if (lvl[g.dst[i]] == -1){
                        lvl[g.dst[i]] = current_lvl + 1;
                        next_frontier.push_back(g.dst[i]);
                    }
                }
                start = g.src[start].first ;
            }
        }
        frontier = next_frontier;
        next_frontier.clear();
        next_frontier.shrink_to_fit();
        current_lvl++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "calculating bfs took : " << duration.count() << '\n' ;
    //print_array(lvl);
};


int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <src> <vertices> " << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const int src = atoi(argv[2]);
    const int vertices = atoi(argv[3]);

    bfs(filename,src,vertices);

    return 0;
}