
#include "../graph-creation/EC.h"
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

void  sequentialSortedBFS(EdgeCentric graph , int source) {
    int size = graph.src.size();
    std::vector level(100,-1) ;
    level[source] = 0 ;
    int curren_level = 0; 
    std::vector<int> frontier;
    std::vector<int> next_frontier;
    frontier.push_back(source);
    while (! frontier.empty())
    {   std::cout << "new_iteration: " <<  std::endl;
        for(int i: frontier) {
            std::cout << "frontier_element " << i  << std::endl;
            auto it = std::find(graph.src.begin(), graph.src.end(), i);
            int index = it - graph.src.begin();
            std::cout << "index: " << index  << std::endl;
            int sum=0;
            for(int j=0 ; j<= index ; j++) {
             sum+= graph.count[j];
            }
            std::cout << "sum: " << sum  << std::endl;
            int next_frontier_size = graph.count[index];
            std::cout << "next_frontier_size: " << next_frontier_size  << std::endl;
            for(int j=sum-next_frontier_size ; j< sum ; j++) {
                std::cout << "neghibor: " << graph.dst[j]  << std::endl;
                if(level[graph.dst[j]] == -1) {
                    next_frontier.push_back(graph.dst[j]);
                    std::cout << "next_frontier_element " << graph.dst[j]  << std::endl;
                    level[graph.dst[j]] = curren_level +1 ;
                }
            }


        }
        frontier = next_frontier;
        next_frontier.clear();
        curren_level++;
    }
    

};

