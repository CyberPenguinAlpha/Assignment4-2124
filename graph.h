#ifndef _graph_h
#define _graph_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "hashTableGraph.h"

typedef struct Vertex
{
	char label;
	int index;
}  Vertex;

typedef struct Graph
{
    hashTable* vertexTable;
    Vertex* vertexArray;
    int numVertices;
    int capacity;

    /* weights of edges are stored in this matrix (0 if edge isn't present) */
    int** pathMatrix;
    int** pathsArray;

}  Graph;


Graph* createGraph( int numVertex );
void freeGraph( Graph* g );

void addVertex( Graph* g, char label );
bool isVertex( Graph* g, char label );
bool getVertexByIndex( Graph* g, int index, char* vertex);

void setEdge( Graph* g, char l1, char l2, int value );
int getEdge( Graph* g, char l1, char l2 );
long getKey( char label );

void printPathMatrix(Graph* g);

#endif
