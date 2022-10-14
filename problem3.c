#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

/*TODO: Notate if you used a partner and the runtime of your implemented algorithm
Partner: Xander Wilson
Big O Time Complexity of below algorithm: O(n³)
Big O Space Complexity of below algorithm: O(n²)
*/

int isStronglyConnected(Graph*);
void initAdjMatrix(Graph*);

int main(){

	/* The following example is from Assignment 4, problem 3 example 1 description (see the pdf for more details). Vertices: {A,B,C,D}, Edges: {(A, B),(A, C),(B, D),(C, A),(D, A)}*/
	Graph* g1 = createGraph(4);
	addVertex(g1,'A');
	addVertex(g1,'B');
	addVertex(g1,'C');
	addVertex(g1,'D');
	setEdge(g1,'A','B',1);
	setEdge(g1,'A','C',1);
	setEdge(g1,'B','D',1);
	setEdge(g1,'C','A',1);
	setEdge(g1,'D','A',1);
	
	/* The following example is from Assignment 4, problem 3 example 2 description (see the pdf for more details). Vertices: {A,B,C,D}, Edges: {(A, B), (B, D), (C, A), (D, A)}*/
	Graph* g2 = createGraph(4);
	addVertex(g2,'A');
	addVertex(g2,'B');
	addVertex(g2,'C');
	addVertex(g2,'D');
	setEdge(g2,'A','B',1);
	setEdge(g2,'B','D',1);
	setEdge(g2,'C','A',1);
	setEdge(g2,'D','A',1);
	
	if (isStronglyConnected(g1)) printf("Strongly Connected.\n");
	else printf("Not Strongly Connected.\n");
	
	if (isStronglyConnected(g2)) printf("Strongly Connected.\n");
	else printf("Not Strongly Connected.\n");
	
	freeGraph(g1);
	freeGraph(g2);

	return 0;
}

int isStronglyConnected(Graph* g){
	
	initAdjMatrix(g);
	int numNodes = g->numVertices;
	
	// apply warshalls
	int i,j,k;                                                                              // O(n³). this might be weird; idk how it's usually done. i literally just walked myself through the algorithm in microsoft paint and coded it accordingly
	for (i = 0; i < numNodes; i++)
		for (j = 0; j < numNodes; j++)
			if (g->pathsArray[j][i] > 0)
				for (k = 0; k < numNodes; k++)
					g->pathsArray[j][k] = g->pathsArray[j][k] || g->pathsArray[i][k];
	
	//check for unreachable nodes
	for (i = 0; i < numNodes; i++)                                                          // O(n²). returns 0 if any nodes are unreachable, otherwise returns 1.
		for (j = 0; j < numNodes; j++)
			if (g->pathsArray[i][j] < 1) return 0;
		
	return 1;
}

void initAdjMatrix(Graph* g){
	
	int numNodes = g->numVertices;
	
	//initialize given graph's path matrix, ignoring weights
	int i,j;
	for (i = 0; i < numNodes; i++)                                                          // O(n²).
		for (j = 0; j < numNodes; j++)
			g->pathsArray[i][j] = (g->pathMatrix[i][j] > 0) ? 1 : 0;
}