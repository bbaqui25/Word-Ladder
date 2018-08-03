/*graph.c*/

//
// Word Ladders using Graphs
//
// Bushra Baqui
// Windows10
// Sublime Text using bert to test
// U. of Illinois, Chicago
// CS251, Fall 2016
// HW #12

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "stack.h"
#include "queue.h"
#include "set.h"
#include "graph.h"
#include "mymem.h"


// #####################################################
//
// Graph:
//

// InputFile() function reads the file, stores it, count # of vertices.
void InputFile(Graph *G, char *filename)
{
	FILE *input;

	char word[50];

	// open file, make sure it worked:
	input = fopen(filename, "r");
	if(input == NULL)
	{
		printf("**Error: unable to open '%s'.\n\n", filename);
		exit(-1);
	}

	printf(">>Building Graph from '%s'...\n", filename);

	while(fscanf(input, "%s\n", word) != EOF) 
	{
		AddVertex(G, word);
	}

	fclose(input);

	return;
}


// WordsOneLetterDiffer() function returns true if the difference between the words in the file differ by 1
int WordsOneLetterDiffer(char *word1, char *word2)
{
	int i = 0;
	int count = 0;

	if(strlen(word1) == strlen(word2))		// if the length of both the words are equal
	{
		for(i = 0; i < strlen(word1); i++) 
		{
			if(word1[i] == word2[i])		// if the characters of both the words are same
			count++;  						// increment count to 1;
		}
		if(count == strlen(word1) - 1)		// if the words differ by 1
	  		return 1;
		else 
	  		return 0;
	}
	else
		return 0;
}


// Lookup() function finds if the given string is a vertex in the graph
int Lookup(Graph *G, char *name)
{
	int i = 0;
	for(i = 0; i < G->NumVertices; i++)
	{
		if(strcmp(G->Names[i], name) == 0)
		{
	  		return i;
		}
	}
	return -1;
}


void ProcessGraph(Graph *G)
{
	int i = 0;
	int j = 0;
	int isValid = 1;
	int v = 0;
	int distance = 0;
	int dist = 0;
	char word[50];
	Vertex *vertex = NULL;
	Vertex *bfs = NULL;

	for (i = 0; i < G->NumVertices; i++)
	{
		for (j = 0; j < G->NumVertices; j++)
		{
	  		if (i != j)
	  		{
	    		if (WordsOneLetterDiffer(G->Names[i], G->Names[j]))
	    		{
	      			if (!AddEdge(G, i, j, 1))
	      			{
	        			printf("**Error: AddEdge failed?!\n\n"); 
	        			exit(-1);
	      			}
	    		}
	  		}
		}
	}

 	PrintGraph(G, "Word Ladder", 0);

 	while (isValid)
	{
		printf("\n>> enter a word (ENTER to quit): ");
		fgets(word, 50, stdin);
		sscanf(word, "%s", word);
	
		if (strcmp(word, "\n") == 0)
	  		isValid = 0;
		else
		{
	  		v = Lookup(G, word);
	  
	  		if (v != -1)
	  		{
	    		vertex = Neighbors(G, v);
	    
	    		printf("** Neighbours:\n   ");		// print neighbours
	    
	    		i = 0;
	    		while (vertex[i] != -1)
	    		{
	      			printf("(%d,%s) ", vertex[i], G->Names[vertex[i]]);
	      			i++;
	    		}

			    printf("\n");
			    printf(">> enter a BFS distance: ");
			    scanf("%d", &distance);
			    printf("** BFS:");
	    
			    bfs = BFSd(G, v, distance);
			    i = 0;
			    dist = 0;
	    
			    while (bfs[i] != -1)
			    {
			    	if (bfs[i] == -2)
			      	{
				        printf("\n   distance %d: ", dist);
				        dist++;
			    	}
			    	else
			        	printf("(%d,%s) ", bfs[i], G->Names[bfs[i]]);
			      
			      	i++;
			    }
			    
			    printf("\n");
			    myfree(vertex);
			    myfree(bfs);
			   
			    getchar();		 // ignore end of line from last integer
			}
			else
				printf("Word not found, please try again...\n");
		}
	}

	DeleteGraph(G);
}


//
// CreateGraph:
//
// Creates a directed graph with an initial capacity for N vertices;
// at this point graph is limited to at most N vertices.  The implementation
// is based on adjacency lists.
//
Graph *CreateGraph(int N)
{
	Graph *G;
	int    i;

	if (N < 1)
	{
		printf("\n**Error in CreateGraph: invalid parameter N (%d)\n\n", N);
		return NULL;
	}

	//
	// allocate graph header:
	//
	G = (Graph *)mymalloc(sizeof(Graph));
	if (G == NULL)
	{
		printf("\n**Error in CreateGraph: malloc failed to allocate\n\n");
		exit(-1);
	}

	//
	// allocate array of adjacency lists, one per vertex:
	//
	G->Vertices = (Edge **)mymalloc(N * sizeof(Edge *));
	if (G->Vertices == NULL)
	{
		printf("\n**Error in CreateGraph: malloc failed to allocate\n\n");
		exit(-1);
	}

	for (i = 0; i < N; ++i)  // initialize to empty lists:
		G->Vertices[i] = NULL;

	//
	// allocate array for storing vertex names:
	//
	G->Names = (char **)mymalloc(N * sizeof(char *));
	if (G->Names == NULL)
	{
		printf("\n**Error in CreateGraph: malloc failed to allocate\n\n");
		exit(-1);
	}

	for (i = 0; i < N; ++i)  // initialize to empty names:
		G->Names[i] = NULL;

	//
	// graph is empty to start --- initialize remaining fields:
	//
	G->NumVertices = 0;
	G->NumEdges = 0;
	G->Capacity = N;

	//
	// done!
	//
	return G;
}


//
// AddVertex:
//
// Adds a vertex with the given name to G, returning a unique integer id
// identifying this vertex.  Returns -1 if adding the vertex failed, i.e.
// if the graph is full and additional vertices cannot be created; this
// occurs when the graph's capacity is reached.
//
int AddVertex(Graph *G, char *name)
{
    int v = G->NumVertices;  // next free location:

    int N = G->Capacity*2;
    if (G->NumVertices == G->Capacity)  // graph is full:
    {

		char **temp1=(char **)mymalloc(sizeof(char *)*N);
		Edge **temp2 = (Edge**)mymalloc(sizeof(Edge*)*N);

		int i;
		for(i =0; i < G->Capacity; i++ )
		{
			temp1[i] = G->Names[i];  //copy names into new array
		}
		int j;
		for(j=0; j < G->Capacity; j++)
		{
			temp2[j] = G->Vertices[j]; //copy Vertices into new array
		}

		//free space
		myfree(G->Names);
		myfree(G->Vertices);

		//copy over the existing vertices into the new arrays
		G->Names = temp1;
		G->Vertices = temp2;

		//update graph Capacity
		G->Capacity = N;
    }
	// there's room, add the vertex:

	//outside of if
	G->Vertices[v] = NULL;

	// initialize edge list to empty:

	// make a copy of the name:
	G->Names[v] = (char *)mymalloc(((int)(strlen(name) + 1)) * sizeof(char));
	if (G->Names[v] == NULL)
	{
		printf("\n**Error in AddVertex: malloc failed to allocate\n\n");
		exit(-1);
	}

	strcpy(G->Names[v], name);

	// one more vertex now:
	G->NumVertices++;

	// done!  Return vertex's number:
	return v;
}


//
// AddEdge:
//
// Adds a directed edge (src, dest, weight) to G.  If successful, true
// (non-zero) is returned; if the edge could not be added (i.e. due to
// invalid vertex ids), then false (0) is returned.
//
// NOTE: loops and multi-edges are allowed.  To allow easier handling of
// multi-edges, edges are stored "in order by destination".  So when
// inserting a new edge E = (s,d,w), insert this edge in the list such
// that E's position in the list is ordered by "d".  Example: if the list
// contains (0,1,100)->(0,2,75), and the new edge is (0,2,150), then the
// resulting list is (0,1,100)->(0,2,150)->(0,2,75).  The new edge is
// inserted *before* existing edges with same or larger destination.
//
int AddEdge(Graph *G, Vertex src, Vertex dest, int weight)
{
	if (src < 0 || src >= G->NumVertices)  // invalid vertex #:
		return 0;
	if (dest < 0 || dest >= G->NumVertices)  // invalid vertex #:
		return 0;

	//
	// allocate memory for new edge:
	//
	Edge *edge = (Edge *)mymalloc(sizeof(Edge));
	if (edge == NULL)
	{
		printf("\n**Error in AddEdge: malloc failed to allocate\n\n");
		exit(-1);
	}

	//
	// store data:
	//
	edge->src = src;
	edge->dest = dest;
	edge->weight = weight;

	//
	// link into edge list --- we want to insert in order so that we
	// can detect multi-edges more easily (i.e. they will be consecutive
	// in the adjacency list):
	//
	Edge *prev = NULL;
	Edge *cur = G->Vertices[src];

	while (cur != NULL)
	{
		if (dest <= cur->dest)  // insert here!
  			break;

		// else keep going:
		prev = cur;
		cur = cur->next;
	}

	if (prev == NULL)  // update head pointer:
	{
		edge->next = G->Vertices[src];
		G->Vertices[src] = edge;
	}
	else  // insert between prev and cur:
	{
		edge->next = prev->next;
		prev->next = edge;
	}

	//
	// done:
	//
	G->NumEdges++;

	return 1;  // success!
}


//
// Neighbors:
//
// Returns the neighbors of a given vertex --- i.e. the vertices
// adjacent to v.  The neighbors are returned in a dynamically-
// allocated array, in ascending order; the vertices are followed
// by -1 to denote the end of the data.  A vertex appears at most
// once in the returned array, even in the presence of multi-edges.
//
// NOTE: returns NULL if v is not a valid vertex id.
//
// NOTE: it is the responsibility of the CALLER to free the returned
// array when they are done.
//
Vertex *Neighbors(Graph *G, Vertex v)
{
	Vertex *neighbors;
	int     N;
	int     i;

	if (v < 0 || v >= G->NumVertices)  // invalid vertex #:
		return NULL;

	//
	// allocate array of worst-case size: # of vertices + 1
	//
	N = G->NumVertices + 1;

	neighbors = (Vertex *)mymalloc(N * sizeof(Vertex));
	if (neighbors == NULL)
	{
		printf("\n**Error in Neighbors: malloc failed to allocate\n\n");
		exit(-1);
	}

	//
	// Now loop through the list of edges and copy the dest
	// vertex of each edge:
	//
	Edge *cur = G->Vertices[v];

	i = 0;
	while (cur != NULL)  // for each edge out of v:
	{
		//
		// the dest is our neighbor --- however, we have to be
		// careful of multi-edges, i.e. edges with the same dest.
		// Since edges are stored in order, edges with same dest
		// appear next to each other in the list --- so look to
		// see if array already contains dest before we copy over:
		//
		if (i == 0)  // first neighbor, we always copy:
		{
			neighbors[i] = cur->dest;
			++i;
		}
		else if (neighbors[i - 1] != cur->dest)  // make sure not a multi-edge:
		{
			neighbors[i] = cur->dest;
			++i;
		}
		else // multi-edge, so ignore:
			;

		cur = cur->next;
  	}

	//
	// follow last element with -1 and return:
	//
	neighbors[i] = -1;

	return neighbors;
}


//
// Prints the graph for debugging purposes.  Pass true
// (non-zero) for the "complete" parameter to dump complete
// info --- e.g. BFS and DFS from each vertex --- otherwise
// pass false (0) for just graph stats.
//
void PrintGraph(Graph *G, char *title, int complete)
{
	printf(">>Graph: %s\n", title);
	printf("  # of vertices: %d\n", G->NumVertices);
	printf("  # of edges:    %d\n", G->NumEdges);

	// is a complete print desired?  if not, return now:
	if (!complete)
		return;

	//
	// Otherwise dump complete graph info:
	//
	printf("  Adjacency Lists:\n");

	int  v;
	for (v = 0; v < G->NumVertices; ++v)
	{
		printf("   %d (%s): ", v, G->Names[v]);

		Edge *edge = G->Vertices[v];
		while (edge != NULL)
		{
			printf("(%d,%d,%d)", edge->src, edge->dest, edge->weight);

			edge = edge->next;
			if (edge != NULL)
				printf(", ");
		}

		printf("\n");
	}

	//
	// neighbors:
	//
	printf("  Neighbors:\n");

	for (v = 0; v < G->NumVertices; ++v)
	{
		printf("   %d (%s): ", v, G->Names[v]);

		Vertex *neighbors = Neighbors(G, v);

		if (neighbors == NULL)
			printf("**ERROR: Neighbors returned NULL.\n\n");
		else
		{
			int  j;

			for (j = 0; neighbors[j] != -1; ++j)
			{
				printf("%d, ", neighbors[j]);
			}

			printf("-1\n");

			myfree(neighbors);
		}
	}

	//
	// BFS:
	//
	printf("  BFS:\n");

	for (v = 0; v < G->NumVertices; ++v)
	{
		printf("   %d (%s): ", v, G->Names[v]);

		Vertex *visited = BFS(G, v);

		if (visited == NULL)
			printf("**ERROR: BFS returned NULL.\n\n");
		else
		{
			int  j;

			for (j = 0; visited[j] != -1; ++j)
			{
				printf("%d, ", visited[j]);
			}

			printf("-1\n");

			myfree(visited);
		}
  	}

	//
	// DFS:
	//
	printf("  DFS:\n");

	for (v = 0; v < G->NumVertices; ++v)
	{
		printf("   %d (%s): ", v, G->Names[v]);

		Vertex *visited = DFS(G, v);

		if (visited == NULL)
			printf("**ERROR: DFS returned NULL.\n\n");
		else
		{
			int  j;

			for (j = 0; visited[j] != -1; ++j)
			{
				printf("%d, ", visited[j]);
			}

			printf("-1\n");

			myfree(visited);
		}
  	}
}


//
// BFS:
//
// Performs a breadth-first search starting from vertex v,
// returning a dynamically-allocated array of the vertices
// visited, in the order they are visited.  Note that v will
// appear first in the returned array, and the last vertex
// is followed by -1 to denote the end.  When the neighbors
// of a vertex are visited, they are done so in ascending
// order; no vertex is visited more than once, even in the
// presence of cycles and multi-edges.
//
// NOTE: returns NULL if v is not a valid vertex id.
//
// NOTE: it is the responsibility of the CALLER to free the
// returned array when they are done.
//
Vertex *BFS(Graph *G, Vertex v)
{
	Vertex *visited;
	int     N;
	int     i;

	if (v < 0 || v >= G->NumVertices)  // invalid vertex #:
		return NULL;

	//
	// allocate array of worst-case size: # of vertices + 1
	//
	N = G->NumVertices + 1;

	visited = (Vertex *)mymalloc(N * sizeof(Vertex));
	if (visited == NULL)
	{
		printf("\n**Error in BFS: malloc failed to allocate\n\n");
		exit(-1);
	}

	//
	// Perform BFS, starting at given vertex v:
	//
	Queue *frontierQ = CreateQueue(N);
	Set   *discoveredSet = CreateSet(N);

	if (!Enqueue(frontierQ, v)) { printf("Error!\n"); exit(-1); }
	if (!AddToSet(discoveredSet, v)) { printf("Error!\n"); exit(-1); }

	i = 0;  // index into visited of where next vertex goes:

	while (!isEmptyQueue(frontierQ))
	{
		Vertex currentV = Dequeue(frontierQ);

		// visit:  add to visited list:
		visited[i] = currentV;
		++i;

		Vertex *neighbors = Neighbors(G, currentV);

		int j = 0;  // index into array of neighbors:
		while (neighbors[j] != -1)
		{
			Vertex adjV = neighbors[j];

			if (!isElementInSet(discoveredSet, adjV))
			{
				if (!Enqueue(frontierQ, adjV)) 
				{ 
					printf("Error!\n"); 
					exit(-1); 
				}
				if (!AddToSet(discoveredSet, adjV)) 
				{ 
					printf("Error!\n"); 
					exit(-1); 
				}
			}

			++j;
		}

	    myfree(neighbors);
	}//while

	//
	// done:
	//
	visited[i] = -1;  // mark end of vertices with -1:

	DeleteQueue(frontierQ);
	DeleteSet(discoveredSet);

	return visited;
}


// BFSd:
// Performs a breadth - first search starting from vertex v, 
// returning a dynamically allocated array of the vertices visited,
// in the order they are visited. Note that v will appear first 
// in the returned array, and the last vertex is followed by -1 
// to denote the end. When the neighbors of a vertex are visited,
// they are done so in ascending order; no vertex is visited more than once,
// even in the presence of cycles and multiedges.
// Note: returns NULL if v is not a valid vertex id.
Vertex *BFSd(Graph *G, Vertex v, int distance)
{
	int dequeued = 0;
	int i = 0;
	int dist = 0;
	int N;
	int count = 0;

	if(v < 0 || v >= G->NumVertices) // invalid vertex #:
		return NULL;

	// allocate array of worst-case size: # of vertices + 1
	N = G->NumVertices + 1;

	int *countDepth = (int *)mymalloc(sizeof(int) * (distance + 1));
	if(countDepth == NULL)
	{
		printf("\n**Error in BFS:\n\n");
		exit(-1);
	}

	for(i = 0; i < distance; i++)
	{
		countDepth[i] = 0;
	}
	countDepth[0] = 1;

	Vertex *visited = (Vertex *)mymalloc(sizeof(Vertex) * (N + distance + 1));
	if(visited == NULL)
	{
		printf("\n**Error in BFS:\n\n");
	}

	// performs BFS, starting at given vertex v:
	Queue *frontierQ = CreateQueue(N);
	Set *discoveredSet = CreateSet(N);

	if(!Enqueue(frontierQ, v))
	{
		printf("Error!\n");
		exit(-1);
	}
	if(!AddToSet(discoveredSet, v))
	{
		printf("Error!\n");
		exit(-1);
	}

	i = 0;
	visited[i] = -2;
	i++;

	while(!isEmptyQueue(frontierQ))
	{
		Vertex currentV = Dequeue(frontierQ);
		dequeued++;

		// visit: add to visited list
		visited[i] = currentV;
		++i;

		Vertex *neighbors = Neighbors(G, currentV);

		int j = 0; // index into array of neighbors
		while(neighbors[j] != -1 && dist < distance)
		{
			Vertex adjV = neighbors[j];

			if(!isElementInSet(discoveredSet, adjV))
			{
				if(!Enqueue(frontierQ, adjV))
				{
					printf("Error!\n");
					exit(-1);
				}
				if(!AddToSet(discoveredSet, adjV))
				{
				  printf("Error!\n");
				  exit(-1);
				}
				count++;
			}
			++j;
    	}

    	if(dist < distance)
      	{
        	if(dequeued == countDepth[dist])
        	{
				countDepth[dist + 1] = count;
				count = 0;
				dist++;
				dequeued = 0;
				visited[i] = -2;
				i++;
        	}
      	}
		myfree(neighbors);
    }		// end of while

    // done:
    visited[i] = -1;		// mark end of vertices with -1:
    myfree(countDepth);
    DeleteQueue(frontierQ);
    DeleteSet(discoveredSet);

    return visited;
}



//
// DFS:
//
// Performs a depth-first search starting from vertex v,
// returning a dynamically-allocated array of the vertices
// visited, in the order they are visited.  Note that v will
// appear first in the returned array, and the last vertex
// is followed by -1 to denote the end; a vertex appears
// at most once in the visited array.  When the neighbors
// of a vertex are visited, they are done so in ascending
// order.
//
// NOTE: returns NULL if v is not a valid vertex id.
//
// NOTE: it is the responsibility of the CALLER to free the
// returned array when they are done.
//
Vertex *DFS(Graph *G, Vertex v)
{
	if (v < 0 || v >= G->NumVertices)  // invalid vertex #:
	return NULL;

	//
	// Perform DFS, starting at given vertex v:
	//
	int N = G->NumVertices + 1;

	Stack *frontierStack = CreateStack(N);
	Set   *visitedSet = CreateSet(N);
	Queue *visitedQ = CreateQueue(N);

	if (!Push(frontierStack, v)) { printf("Error!\n"); exit(-1); }

	while (!isEmptyStack(frontierStack))
	{
		Vertex currentV = Pop(frontierStack);

		//
		// visit:  add to visited list *if* not already there, since
		// DFS may end up visiting vertices multiple times:
		//
		if (!isElementInQueue(visitedQ, currentV))
		{
			if (!Enqueue(visitedQ, currentV)) 
			{ 
				printf("Error!\n"); 
				exit(-1); 
			}
		}

		//
		// now push neighbors in reverse order so that we visit in
		// ascending order:
		//
		if (!isElementInSet(visitedSet, currentV))
		{
			if (!AddToSet(visitedSet, currentV)) 
			{ 
				printf("Error!\n"); 
				exit(-1); 
			}

			Vertex *neighbors = Neighbors(G, currentV);

			//
			// Note: push them backwards onto stack so vertices are
			// on the stack in ascending order:
			//
			int j = 0;  // index into array of neighbors:

			while (neighbors[j] != -1)  // find the end
				j++;

			j--;  // advance to last vertex in the visited array:

			while (j >= 0)  // now push backwards:
		  	{
		    	Vertex adjV = neighbors[j];

		    	if (!Push(frontierStack, adjV)) 
		    	{ 
		    		printf("Error!\n"); 
		    		exit(-1); 
		    	}

		    	--j;
			}

			myfree(neighbors);
		}
	}//while

	//
	// done: our set of visited vertices is in visitedQ, so let's
	// copy out of there into a dynamically-allocated array:
	//

	//
	// allocate array of worst-case size: # of vertices + 1
	//
	Vertex *visited;

	visited = (Vertex *)mymalloc(N * sizeof(Vertex));
	if (visited == NULL)
	{
		printf("\n**Error in DFS: malloc failed to allocate\n\n");
		exit(-1);
	}

	// dequeue vertices into the visited array:
	int i = 0;  // index into visited array:

	while (!isEmptyQueue(visitedQ))
	{
		visited[i] = Dequeue(visitedQ);
		++i;
	}

	visited[i] = -1;  // mark end of vertices with -1:

	//
	// Done:
	//
	DeleteStack(frontierStack);
	DeleteSet(visitedSet);
	DeleteQueue(visitedQ);

	return visited;
}


// DeleteGraph() funcion frees all the memory
void DeleteGraph(Graph *G)
{
	int i;

	//free each vertex Names
	for(i = 0; i < G->NumVertices; i++)
	{
		myfree(G->Names[i]);
	}
	myfree(G->Names); //free underlying Names array

	int j;

	//free the Edge in each Vertices Linked List
	for(j = 0; j < G->NumVertices; j++)
	{
		Edge *cur = G->Vertices[j];
		while(cur != NULL)
		{
			myfree(cur);
			cur = cur->next;
		}
	}
	myfree(G->Vertices);
	myfree(G);
}

