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
#include <string.h>

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
    for(size_t i = 0; i < size; i++) {
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
    node = (GraphNode *) malloc(sizeof(GraphNode));
    Iter *iterator = ol_iterator(graph);
    
    //for each node in the graph
    for(size_t i = 0; i < size; i++) {
        node = (GraphNode *) ol_next(iterator);
        printf("%s:", node->name);
        
        //if node has neighbors
        if(node->neighbors != NULL) {
            //initialize variables to traverse neighbors
            char *neigh;
            Iter *neigh_iter = ol_iterator(node->neighbors);
            printf(" "); //space after the colon

            //for each neighbor
            for(size_t j = 0; j < ol_get_size(node->neighbors); j++) {
                //add comma at the end if there is another neighbor
                if(ol_has_next(neigh_iter)) {
                    neigh = (char *) ol_next(neigh_iter);
                    printf("%s, ", neigh);
                }
                //don't add comma if last neighbor
                else{
                    neigh = (char *) ol_next(neigh_iter);
                    printf("%s", neigh);
                }
            } //end of traversing neighbors
            free(neigh_iter);
        } //end of if have neighbors
        printf("\n");
    }//end of traversing graph

    free(iterator);
    free(node);
}

/// neighbor_exists determines whether a neighbor with a given name is in the list
/// @param neighbors is a pointer to the list of neighbors
/// @param name is the name of the neighbor
static int neighbor_exists(ObjectList *neighbors, char *name) {
    Iter *iterator = ol_iterator(neighbors);
    char *tmp;
    while(ol_has_next(iterator)) {
        tmp = (char *) ol_next(iterator);
        if(strcmp(name, tmp) == 0) {
            free(iterator);
            return 1;
        }
    }
    free(iterator);
    return 0;

}

///loads a input from a stream and puts it into the graph
void grh_load_file(ObjectList *graph, FILE *input) {
    char line[MAX_FILE_LINE_LENGTH];
    fgets(line, MAX_FILE_LINE_LENGTH, input);
    //There is actual input
    if(strlen(line) > 1) {
        char strip_line[MAX_FILE_LINE_LENGTH] = "";
        strncpy(strip_line, line, strlen(line)-1);
        GraphNode *node = (GraphNode *) malloc(sizeof(GraphNode));
        char comma[2] = ",";
        char *neighbors = strchr(strip_line, ',');
        //if it has neighbors
        if(neighbors != NULL) {
            char name[MAX_NAME] = "";
            strncpy(name, line, (size_t)(neighbors-strip_line));
            node = grh_find_node_by_name(graph, name);
            //if node not exist create one
            if(node == NULL) {
                node = grh_create_node(name);
                ol_insert(graph, node);
            }
            neighbors ++; //make index point to after the comma
            char *token = strtok(neighbors, comma); //separating neighbors
            //add neighbors until empty
            while(token != NULL) {
                GraphNode *tmp = (GraphNode *) malloc(sizeof(GraphNode));
                tmp = grh_find_node_by_name(graph, token);
                //if neighbor not in graph add new node
                if(tmp == NULL) {
                    tmp = grh_create_node(token);
                    ol_insert(tmp->neighbors, token);
                    ol_insert(graph, tmp);
                }
                //neighbor is in graph, insert the first name as a neighbor if need
                else if(!neighbor_exists(tmp->neighbors, name)) {
                    ol_insert(tmp->neighbors, name);
                }
                //insert neighbor for first name if not there
                if(!neighbor_exists(node->neighbors, token)) {
                    ol_insert(node->neighbors, token);
                }
                free(tmp);
                token = strtok(NULL, comma);
            }    
        }
        //if no neighbors
        else {
            node = grh_find_node_by_name(graph, strip_line);
            if(node == NULL) {
                node = grh_create_node(strip_line);
                ol_insert(graph, node);
            }
        }
        free(node);
    } //there is no input so skip blank line
}
