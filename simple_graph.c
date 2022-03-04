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
#include <assert.h>

/// A node representing an adjcency list with a varying size of neighbors
GraphNode * grh_create_node(char *name) {
    assert(name);
    GraphNode *node;
    node = (GraphNode *) malloc(sizeof(GraphNode));
    strcpy(node->name, name);
    node->neighbors = ol_create();
    return node;
}

/// deletes given node including struct and its fields
void grh_delete_node(GraphNode *node) {
    assert(node);
    Iter *neigh_iter = ol_iterator(node->neighbors);
    while(ol_has_next(neigh_iter)) {
        free(ol_next(neigh_iter));
    }
    ol_destroy(node->neighbors);
    free(neigh_iter);
    free(node);
}

/// deletes the given graph
void grh_delete_graph(ObjectList *graph) {
    assert(graph);
    Iter *iterator = ol_iterator(graph);
    while(ol_has_next(iterator)) {
        grh_delete_node(ol_next(iterator));
    }
    ol_destroy(graph);
    free(iterator);
}

/// Given an objectlist, returns the node with the given name or NULL
GraphNode * grh_find_node_by_name(ObjectList *graph, char *name) {
    assert(name);
    assert(graph);
    Iter *iterator = ol_iterator(graph);
    while(ol_has_next(iterator)) {
        GraphNode *node = (GraphNode *)ol_next(iterator);
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
    assert(graph);
    //initialize variables to traverse graph
    size_t size = ol_get_size(graph);
    GraphNode *node;
    Iter *iterator = ol_iterator(graph);
    
    //for each node in the graph
    for(size_t i = 0; i < size; i++) {
        node = (GraphNode *) ol_next(iterator);
        printf("%s:", node->name);
        
        size_t neigh_size = ol_get_size(node->neighbors);
        //if node has neighbors
        if(neigh_size > 0) {
            //initialize variables to traverse neighbors
            char *neighbor;
            Iter *neigh_iter = ol_iterator(node->neighbors);
            printf(" "); //space after the colon
            
            for(size_t j = 0; j < (neigh_size-1); j++) {
                //add comma at the end if there is another neighbor
                neighbor = ol_next(neigh_iter);
                printf("%s, ", neighbor);
            }
            //print last neighbor
            neighbor = ol_next(neigh_iter);
            printf("%s", neighbor);
            free(neigh_iter);
        } //end of if have neighbors
        printf("\n");
    }//end of traversing graph
    free(iterator);
}

/// neighbor_exists determines whether a neighbor with a given name is in the list
/// @param neighbors is a pointer to the list of neighbors
/// @param name is the name of the neighbor
/// @pre-condition neighbors, neigh, and name are not NULL
static int neighbor_exists(ObjectList *neighbors, const char *neigh, const char *name) {
    assert(neighbors);
    assert(neigh);
    assert(name);
    //if the neighbor is the same as the base state
    if(strcmp(neigh, name) == 0) {
        return 1;
    }

    //iterate neighbors and see if it already exists
    Iter *iterator = ol_iterator(neighbors);
    char *tmp;
    while(ol_has_next(iterator)) {
        tmp = (char *) ol_next(iterator);
        if(strcmp(neigh, tmp) == 0) {
            free(iterator);
            return 1;
        }
    }
    free(iterator);
    return 0;

}

///loads a input from a stream and puts it into the graph
void grh_load_file(ObjectList *graph, FILE *input) {
    assert(graph);
    assert(input);

    char line[MAX_FILE_LINE_LENGTH] = "";
    fgets(line, MAX_FILE_LINE_LENGTH, input);
    //There is actual input
    if(strlen(line) > 1) {
        char strip_line[MAX_FILE_LINE_LENGTH] = "";
        strncpy(strip_line, line, strlen(line)-1);
        GraphNode *node;
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
                assert(node);
                ol_insert(graph, node);
            }
            neighbors ++; //make index point to after the comma
            char *token = strtok(neighbors, comma); //separating neighbors
            //add neighbors until empty
            while(token != NULL) {
                GraphNode *tmp;
                tmp = grh_find_node_by_name(graph, token);
                //if neighbor not in graph add new node
                if(tmp == NULL) {
                    tmp = grh_create_node(token);
                    assert(tmp);
                    char *node_name = (char *) malloc(strlen(name) + 1);
                    strcpy(node_name, name);
                    ol_insert(tmp->neighbors, node_name);
                    ol_insert(graph, tmp);
                }
                //neighbor is in graph, insert the first name as a neighbor if need
                else if(!neighbor_exists(tmp->neighbors, name, token)) {
                    char *node_name = (char *) malloc(strlen(name) + 1);
                    strcpy(node_name, name);
                    ol_insert(tmp->neighbors, node_name);
                }
                //insert neighbor for first name if not there
                if(!neighbor_exists(node->neighbors, token, name)) {
                    char *neigh = (char *) malloc(strlen(token) + 1);
                    strcpy(neigh, token);
                    ol_insert(node->neighbors, neigh);
                }
                token = strtok(NULL, comma);
            }    
        }
        //if no neighbors
        else {
            node = grh_find_node_by_name(graph, strip_line);
            if(node == NULL) {
                node = grh_create_node(strip_line);
                assert(node);
                ol_insert(graph, node);
            }
        }
    } //there is no input so skip blank line
}
