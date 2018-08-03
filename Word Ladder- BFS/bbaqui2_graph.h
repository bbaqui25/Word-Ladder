/*graph.h*/

//
// Graph:
//
typedef int Vertex;

typedef struct Edge
{
	Vertex  src;
	Vertex  dest;
	int     weight;
	struct Edge *next;
} Edge;

typedef struct Graph
{
	Edge **Vertices;
	char **Names;
	int    NumVertices;
	int    NumEdges;
	int    Capacity;
} Graph;

Graph  *CreateGraph(int N);
int     AddVertex(Graph *G, char *name);
int     AddEdge(Graph *G, Vertex src, Vertex dest, int weight);
Vertex *Neighbors(Graph *G, Vertex v);
void    PrintGraph(Graph *G, char *title, int complete);
Vertex *BFS(Graph *G, Vertex v);
Vertex *DFS(Graph *G, Vertex v);
void DeleteGraph(Graph *G);
void InputFile(Graph *G, char *filename);
int WordsOneLetterDiffer(char *word1, char *word2);
int Lookup(Graph *G, char *name);
void ProcessGraph(Graph *G);
Vertex *BFSd(Graph *G, Vertex v, int distance);
