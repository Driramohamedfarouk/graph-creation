//
// Created by farouk on 17/07/23.
//

#include <fstream>

#include "ChainedEC.h"
#include "../../utils/timer.h"
#include "../../utils/result_writer.h"

#define alpha 15
#define beta 18

// This is a BFS implementation using the Direction-Optimization technique from [1].

// in the top-down approach we use a simple array of fixed size to represent the frontier,
// we used an optimization technique from Merrill et al. [2] which computes using
// Prefix sums of degrees memory locations each thread  will write to,
// therefore eliminating the need for atomic updates to the frontier.


// [1] Scott Beamer, Krste Asanović, and David Patterson. "Direction-Optimizing
// Breadth-First Search." International Conference on High Performance
// Computing, Networking, Storage and Analysis (SC), Salt Lake City, Utah,
//        November 2012.

// [2] Duane Merrill, Michael Garland, and Andrew Grimshaw. 2012.
// Scalable GPU graph traversal. SIGPLAN Not. 47, 8 (August 2012), 117–128.

int bottom_up_step(ChainedEdgeCentric& g, int* lvl , int current_lvl ){
    node_t* head ;
    int dst ;
    // use reduction to compute frontier size
    int nextFrontierSize = 0 ;
    #pragma omp parallel for reduction(+:nextFrontierSize) private(head,dst) schedule(dynamic,128)
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


int top_down_step(ChainedEdgeCentric& g, int* lvl , int* frontier ,int* frontierSize, int* next_frontier,  int current_lvl, int* degrees){
    node_t *head ;
    int sum = 0 , d ;
    for (int i = 0; i < *frontierSize ; ++i) {
        degrees[i] = sum ;
        sum+=g.out_degree[frontier[i]] ;
    }
    int prev , dst ;
    #pragma omp parallel for schedule(dynamic,1024) private(prev,d,head,dst)
    for(int i =  0 ; i< *frontierSize ;++i) {
        head = g.head[frontier[i]].next ;
        d = 0 ;
        while (head){
            dst = head->dst ;
            if (lvl[dst]==-1) {
            #pragma omp atomic capture
                {
                    prev = lvl[dst];
                    lvl[dst] = current_lvl ;
                }
                if(prev==-1)
                    next_frontier[degrees[i] + d ] = dst ;
            } else {
                next_frontier[degrees[i] + d ] = -1;
            }
            d++ ;
            head= head->next ;
        }
    }
    *frontierSize = 0 ;
    for (int i = 0; i < sum ; ++i) {
        if(next_frontier[i]!=-1) {
            frontier[(*frontierSize)++] = next_frontier[i] ;
        }
    }
    return sum;
}




void bfs(const std::string& path, int src, const  int n , const int m) {

    ChainedEdgeCentric g = createChainedEdgeCentric(path, n,m);
    //std::cout << "loaded the graph in memory " << '\n' ;

    Timer timer ;
    timer.Start();

    auto *lvl = new int[n];

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        lvl[i] = -1 ;
    }

    int *degrees = new int[n] ;

    int current_lvl = 0;

    lvl[src] = 0;

    int * frontier, *next_frontier;
    // TODO : can limit those sizes with alpha and beta
    frontier = new int[n] ;
    next_frontier = new int[m] ;
    frontier[0] = src ;
    int frontierSize = 1 ;
    int m_u = g.edges , m_f = g.out_degree[src] ;
    do {
        current_lvl++ ;
        m_f =  top_down_step(g,lvl,frontier,&frontierSize,next_frontier,current_lvl,degrees);
        m_u -= m_f ;
        std::cout << " TD : frontier size in level " << current_lvl << " = " << frontierSize << '\n' ;
    }while (m_f<=m_u/alpha);
    int prev_frontier ;
    do{
        prev_frontier = frontierSize ;

        frontierSize = bottom_up_step(g,lvl,current_lvl);
        current_lvl++ ;
        std::cout << " BU : frontier size in level " << current_lvl << " = " << frontierSize << '\n' ;
    } while ( frontierSize > prev_frontier || frontierSize>=g.vertices/beta) ;
    frontierSize = 0 ;
    for (int i = 0; i < n; ++i) {
        if(lvl[i]== current_lvl){
            frontier[frontierSize++] = i  ;
        }
    }
    while (frontierSize>0) {
        current_lvl++;
        top_down_step(g,lvl,frontier,&frontierSize,next_frontier,current_lvl,degrees);
        std::cout << " TD : frontier size in level " << current_lvl << " = " << frontierSize << '\n' ;
    }

    timer.Stop();
    std::cout << "calculating bfs took : " << timer.Millisecs() << '\n' ;

    Writer<int>::write("DOBFSOutput.txt",lvl,n)  ;

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