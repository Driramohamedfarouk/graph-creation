//
// Created by farouk on 17/07/23.
//

#include <fstream>

#include "ChainedEC.h"
#include "../../utils/timer.h"
#include "../../utils/result_writer.h"

#define alpha 15
#define beta 18



int bottom_up_step(ChainedEdgeCentric& g, int* lvl , int current_lvl ){
    node_t* head ;
    int dst ;
    // use reduction to compute frontier size
    int nextFrontierSize = 0 ;
    #pragma omp parallel for reduction(+:nextFrontierSize) private(head,dst)
    for (int i = 0; i < g.vertices; ++i) {
           if(lvl[i]<0) {
               head = g.head_i[i].next ;
               while (head){
                   dst = head->dst ;
                   if(lvl[dst] == current_lvl){
                       // add i in the next frontier
                       lvl[i] = current_lvl+1 ;
                       nextFrontierSize++ ;
                       break;
                   }
                   head= head->next ;
               }
           }
    }
   return nextFrontierSize ;
}


void bfs(const std::string& path, int src, const  int n , const int m) {


    ChainedEdgeCentric g = createChainedEdgeCentric(path, n,m);
    std::cout << "loaded the graph in memory " << '\n' ;
    Timer timer ;
    timer.Start();
    auto *lvl = new int[n];

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        lvl[i] = -1 ;
    }

    int current_lvl = 0;

    node_t* head ;
    lvl[src] = 0;

    int frontierSize = 1 , dst ;
    while (frontierSize>0) {
        int nextFrontierSize = 0 ;
        #pragma omp parallel for reduction(+:nextFrontierSize) private(head,dst)
        for (int i = 0; i < g.vertices; ++i) {
            if(lvl[i]<0) {
                head = g.head_i[i].next ;
                while (head){
                    dst = head->dst ;
                    if(lvl[dst] == current_lvl){
                        // add i in the next frontier
                        lvl[i] = current_lvl+1 ;
                        nextFrontierSize++ ;
                        break;
                    }
                    head= head->next ;
                }
            }
        }
        frontierSize = nextFrontierSize ;
        current_lvl++ ;
    }
    timer.Stop();
    std::cout << "calculating bfs took : " << timer.Millisecs() << '\n' ;

    Writer<int>::write("BUBFSOutput.txt",lvl,n)  ;

}

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