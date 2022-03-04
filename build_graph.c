//
// File: build_graph.c
// build_graph.c builds a graph given input from stdin
//
// @author Ming Creekmore mec5765
//
// @date 3/3/2022
// // // // // // // // // // // // // // // // // // // // // //

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "simple_graph.h"

/// main reads in data from stdin, builds the graph, then prints it
/// return EXIT_SUCCESS if succeeed
int main(void) {
    //making graph
    printf("Reading graph data...\n");
    ObjectList *graph = ol_create();
    while(!feof(stdin)) {
        grh_load_file(graph, stdin);
    }

    //printing graph
    printf("Finished reading graph data.\n");
    printf("Graph: {name: neighbor names list}\n");
    grh_print_graph(graph);
    grh_delete_graph(graph);
    return EXIT_SUCCESS;
}
