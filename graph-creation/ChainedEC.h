//
// Created by farouk on 19/07/23.
//

#ifndef GRAPH_PROCESSING_FRAMEWORK_ICHAINEDEC_CPP_H
#define GRAPH_PROCESSING_FRAMEWORK_ICHAINEDEC_CPP_H
#include <iostream>
#include <vector>

typedef struct node_t node_t;

typedef struct node_t {
    int count ;
    node_t *next ;
} node_t ;

struct ChainedEdgeCentric{
    node_t* indexing;
    node_t* src ;
};


#endif //GRAPH_PROCESSING_FRAMEWORK_ICHAINEDEC_CPP_H
