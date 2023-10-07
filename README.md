# Graph Processing Framework 
This project explores big graph processing on multi-core machine.
Existing systems rely on sorted edge lists for efficient graph
loading in CSR format. Our project's key contribution lies in 
investigating the feasibility of eliminating edge list sorting.
We introduce a novel data structure to represent graphs in memory, 
easy to construct and that requires no preprocessing.

## Data structure description 
![ok.png](..%2F..%2FDownloads%2Fok.png)

## Algorithms implemented 
### bfs 
### PageRank
### Connected Components
### Betweennes Centrality

## Contribution 
There are two implementation optimizations that can be done to improve the data structure
* Every cell in the array is referenced by a unique pointer. By finding ways to implicitly tell the compiler that there is no pointer aliasing (something like __restrict keyword) we will give his freedom to optimize further (try with clang and GCC). 
* Introduce software pipelines to hide latency caused pointer jumping while fetching the neighbours of a node. More precisely, In parallel one thread executes work on current neighbours of a node while in parallel another, jumps in memory and prepare the neighbours of the next node to be executed.

## References

[1] Scott Beamer, Krste Asanović, David Patterson. The GAP Benchmark Suite. arXiv:1508.03619 [cs.DC], 2015.   
[2] Scott Beamer, "Understanding and Improving Graph Algorithm Performance", Ph.D. Thesis, University of California Berkeley, September 2016.  
[3] Scott Beamer, Krste Asanović, and David Patterson. "Direction-Optimizing
Breadth-First Search." International Conference on High Performance
Computing, Networking, Storage and Analysis (SC), Salt Lake City, Utah, November 2012.  
[4] Duane Merrill, Michael Garland, and Andrew Grimshaw. 2012.
Scalable GPU graph traversal. SIGPLAN Not. 47, 8 (August 2012), 117–128.  
[5] Ulrik Brandes: A Faster Algorithm for Betweenness Centrality, Journal of Mathematical Sociology 2001.  
[6] Yossi Shiloach and Uzi Vishkin. An O(log N) parallel connectivity algorithm. Journal of Algorithms, 3:57-67, 1982.
