//
// Created by farouk on 17/07/23.
//

#include <fstream>

#include "ChainedEC.h"
#include "../../utils/timer.h"
#include "../../utils/result_writer.h"


void bfs(const std::string& path, int src, const  int n,const int nb_edges ) {

    ChainedEdgeCentric g = createChainedEdgeCentric(path, n,nb_edges);
    std::cout << "loaded the graph in memory " << '\n' ;

    Timer timer ;
    timer.Start();
    auto *lvl = new int[n];

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        lvl[i] = -1 ;
    }

    int *degrees = new int[n] ;

    int current_lvl = 0;

    node_t* head ;
    lvl[src] = 0;

    int * frontier, *next_frontier;
    frontier = new int[n] ;
    next_frontier = new int[nb_edges] ;
    frontier[0] = src ;
    int frontierSize = 1 , d , sum ;
    while (frontierSize>0) {
        std::cout << "frontier size -> " << frontierSize << '\n' ;
        current_lvl++;
        // construct an array of frontier prefixed degrees
        sum = 0 ;
        for (int i = 0; i < frontierSize ; ++i) {
            degrees[i] = sum ;
            sum+=g.out_degree[frontier[i]] ;
        }
        int prev , dst ;
        #pragma omp parallel for schedule(dynamic,1024) private(prev,d,head,dst)
        for(int i =  0 ; i< frontierSize ;++i) {
            head = g.indexing[frontier[i]].next ;
            d = 0 ;
            while (head){
                    dst = head->dst ;
                    if (lvl[dst]==-1){
                        #pragma omp atomic capture
                        {
                            prev = lvl[dst];
                            lvl[dst] = current_lvl ;
                        }
                        if(prev==-1)
                            next_frontier[degrees[i] + d ] = dst ;
                    } else {
                        next_frontier[degrees[i] + d] = -1;
                    }
                    d++ ;
                    head= head->next ;
            }
        }
        frontierSize = 0 ;
        for (int i = 0; i < sum ; ++i) {
            if(next_frontier[i]!=-1) {
                frontier[frontierSize++] = next_frontier[i] ;
            }
        }
    }
    timer.Stop();
    std::cout << "calculating bfs took : " << timer.Millisecs() << '\n' ;

    Writer<int>::write("BFSOutput.txt",lvl,n)  ;

};


int main(int argc, char* argv[]) {

    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <filename> <vertices> <edges> <src> " << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const int vertices = atoi(argv[2]);
    const int edges = atoi(argv[3]);
    const int src = atoi(argv[4]);

    bfs(filename,src,vertices,edges);

    return 0;
}