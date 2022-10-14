#include "graph.h"

int getIndex( Graph* g, char label );
long minLong( long a, long b );

/* create
 * input: an int representing the maximum number of vertices needed in your graph
 * output: a pointer to a Graph (this is malloc-ed so must be freed eventually)
 *
 * Creates a new empty Graph and returns a pointer to it.  The maximum number of vertices is set to capacity.
 */
Graph* createGraph( int capacity )
{
    int i,j;
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->vertexArray = (Vertex*)malloc(sizeof(Vertex)*capacity);
    g->vertexTable = createTableType( capacity, FIBONACCI_HASH, false );

    g->pathMatrix = (int**)malloc(sizeof(int*)*capacity);
    g->pathsArray = (int**)malloc(sizeof(int*)*capacity);

    for( i=0; i<capacity; i++ ){
        g->pathMatrix[i] = (int*)malloc(sizeof(int)*capacity);
        g->pathsArray[i] = (int*)malloc(sizeof(int)*capacity);
    }

    for( i=0; i<capacity; i++ ){
        for( j=0; j<capacity; j++ ){
            g->pathMatrix[i][j] = 0;
            g->pathsArray[i][j] = -1;
        }
    }

    g->numVertices = 0;
    g->capacity = capacity;
    return g;
}

/* freeGraph and freePathList
 * input: a pointer to a Graph
 * output: none
 *
 * frees the given Graph and all of it's Vertex elements
 */
void freeGraph( Graph* g )
{
    int i;

    if( g==NULL ){
        printf("ERROR - freeGraph - Attempt to free NULL Graph pointer\n");
        exit(-1);
    }

    for( i=0; i<g->capacity; i++ ){
        free(g->pathMatrix[i]);
        free(g->pathsArray[i]);
    }

    free( g->pathMatrix );
    free( g->pathsArray );

    /* Free vertex table */
    freeTable( g->vertexTable );

    /* Free vertex array */
    free( g->vertexArray );
    free( g );
}

/* addVertex
 * input: a pointer to a Graph, a char label
 * output: none
 *
 * adds a new vertex to the graph with its data set to p
 */
void addVertex( Graph* g, char label ){
    int i = getIndex( g, label );
    if( g->numVertices==g->capacity ){
        printf("ERROR - addVertex - Attempt to add more vertices than the capacity of the graph\n");
        exit(-1);
    }

    /* Check if vertex already in graph */
    if( i!=-1 ){
        printf("WARNING - addVertex - Attempt to add same label to graph multiple times\n");
        return;
    }

    g->vertexArray[g->numVertices].label = label;
    g->vertexArray[g->numVertices].index = g->numVertices;

    insertTable( g->vertexTable, getKey( label ), &g->vertexArray[g->numVertices] );

    g->numVertices++;
}

/* isVertex
 * input: a pointer to a Graph, a char label
 * output: a bool
 *
 * returns true if the given char is contained in the graph
 */
bool isVertex( Graph* g, char label ){
    return getIndex( g, label )!=-1;
}

/* getVertexByIndex
 * input: a pointer to a Graph, an int, a pointer to a char
 * output: a bool
 *
 * finds index-th vertex added to the graph and inserts into next.  It returns true if this search was successful and false otherwise.
 */
bool getVertexByIndex( Graph* g, int index, char* vertex ){
    if( index>=g->numVertices )
        return false;
    *vertex = g->vertexArray[index].label;
    return true;
}

/* getIndex
 * input: a pointer to a Graph, a char
 * output: a bool
 *
 * finds index-th vertex added to the graph and inserts into next.  It returns true if this search was successful and false otherwise.
 */
int getIndex( Graph* g, char label ){
    Vertex* v = (Vertex *)searchTable( g->vertexTable, getKey(label) );
    if( v==NULL )
        return -1;
    return v->index;
}

/* setEdge
 * input: a pointer to a Graph, two char variables, a int
 * output: none
 *
 * Sets the specified edge in the graph to the int value
 */
void setEdge( Graph* g, char l1, char l2, int value )
{
    int i = getIndex( g, l1 );
    int j = getIndex( g, l2 );

    /* Add vertices if they're not already in graph */
    if( i==-1 ){
        addVertex( g, l1 );
        i = getIndex( g, l1 );
    }
    if( j==-1 ){
        addVertex( g, l2 );
        j = getIndex( g, l2 );
    }
    g->pathMatrix[i][j] = value;
}

/* getEdge
 * input: a pointer to a Graph, two char variables
 * output: a int
 *
 * Returns whether the specified edge is in the graph
 */
int getEdge( Graph* g, char l1, char l2 )
{
    int i = getIndex( g, l1 );
    int j = getIndex( g, l2 );

    /* Add vertices if they're not already in graph */
    if( i==-1 ){
        addVertex( g, l1 );
        i = getIndex( g, l1 );
    }
    if( j==-1 ){
        addVertex( g, l2 );
        j = getIndex( g, l2 );
    }

    return g->pathMatrix[i][j];
}

long getKey( char label ){
    long key = (long) label;
    return key;
}

void printPathMatrix(Graph* g){
	int i,j;
	for(i = 0; i < g->numVertices; i++){
		for(j = 0; j < g->numVertices; j++){
			printf("%d  ",g->pathMatrix[getIndex(g,g->vertexArray[i].label)][getIndex(g,g->vertexArray[j].label)]);
		}
		printf("\n");
	}
}

