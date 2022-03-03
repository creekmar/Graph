//
// File: simple_graph.c
// simple_graph.c implements the interface simple_graph.h
// 
// @author Ming Creekmore mec5765
//
// @date 3/3/2022
//
// // // // // // // // // // // // // // // // // // // // // // 

#include "simple_graph.h"

/// A node representing an adjcency list with a varying size of neighbors
GraphNode * grh_create_node(char *name) {
    GraphNode *node;
    node = (GraphNode *) malloc(sizeof(GraphNode));
    if(node != NULL ) {
        strcpy(node->name, name);
        node->neighbors = ol_create();
    }
    return node;
}

/// deletes given node including struct and its fields
void grh_delete_node(GraphNode *node) {
    Iter *iterator = ol_iterator(node->neighbors);
    while(ol_has_next(iterator)) {
        free(ol_next(iterator));
    }
    ol_destroy(node->neighbors);
    free(node);
    free(iterator);
}

/// deletes the given graph
void grd_delete_graph(ObjectList *graph) {
    Iter *iterator = ol_iterator(graph);
    while(ol_has_next(iterator)) {
        grh_delete_node(ol_next(iterator));
    }
    ol_destroy(graph);
    free(iterator);
}

/// Given an objectlist, returns the node with the given name or NULL
GraphNode * grh_find_node_by_name(ObjectList *graph, char *name) {
    size_t size = ol_get_size(graph);
    GraphNode *node;
    node = (GraphNode *) malloc(sizeof(GraphNode));
    Iter *iterator = ol_iterator(graph);
    for(int i = 0; i < size; i++) {
        node = (GraphNode *)ol_next(iterator);
        if(strcmp(node->name, name) == 0) {
            free(iterator);
            return node;
        }
    }
    free(iterator);
    return NULL;
}

/// Prints a given graph
void grh_print_graph(ObjectList *graph) {
    //initialize variables to traverse graph
    size_t size = ol_get_size(graph);
    GraphNode *node;
    node = (GraphNode *) malloc(sizeof(GrpahNode));
    Iter *iterator = ol_iterator(graph);
    
    //for each node in the graph
    for(int i = 0; i < size; i++) {
        node = (GraphNode *) ol_next(iterator);
        printf("%s:", node->name);
        
        //if node has neighbors
        if(node->neighbors != NULL) {
            //initialize variables to traverse neighbors
            GraphNode *neigh;
            neigh = (GraphNode *) malloc(sizeof(GraphNode));
            Iter *neigh_iter = ol_iterator(node->neighbors);
            printf(" "); //space after the colon

            //for each neighbor
            for(int j = 0; j < ol_get_size(node->neighbors); j++) {
                //add comma at the end if there is another neighbor
                if(ol_has_next(neigh_iter)) {
                    neigh = (char *) ol_next(neigh_iter);
                    printf("%s, ", neigh->name);
                }
                //don't add comma if last neighbor
                else{
                    neigh = (char *) ol_next(neigh_iter);
                    printf("%s", neigh->name);
                }
            } //end of traversing neighbors
            free(neigh_iter);
            free(neigh);
        } //end of if have neighbors
        printf("\n");
    }//end of traversing graph

    free(iterator);
    free(node);
}

void grh_load_file(ObjectList *graph, FILE *input) {
    
}
