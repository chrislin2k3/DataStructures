#include <stdio.h>
#include <stdlib.h>
#include "03-Queue.h"

/*
1. Graph
    graph_t* createGraph(int numVertex, int  directed);
    graph_t* insertVertex(graph_t *G, int vertex)
    graph_t* insertEdge(graph_t *G, int src, int vtx, int weight)
    Graph_t* deleteVertex(graph_t *G, int vertext)
    graph_t* deleteEdge(graph_t *G, int src, int vtx) 
    boolean  isEmpty(graph_t *G)
    graph_t* listAdjacent(graph_t *G, int) 
2. Elementary Graph Operation
    Depth First Search (DFS)
    Breadth First Search (BFS)
    Connected Components
    Spanning Trees
    Biconnected Componets
3. Minimum Cost Spanning Trees   
    kruskal's Algorithm
    Prim's Algorithm
    Sollin's Algorithm
4. Shortest Path and Transitive Closure
    Single Source/All Destination: Non Negative Edge Cost
    Single Source/All Destination: General Weights
    All Paired Shortest Paths
    Transitive Closure
5. ACtivity Networks   
    Activity-on-Vertex (AOV) Networks
    Activity-on-Edge (AOE) Networks
*/

#define MAX_VERT 8
#define MAX_COST 9999
#define TRUE  1
#define FALSE 0

int discovered[MAX_VERT];
int visited[MAX_VERT];
int distance[MAX_VERT];
int found[MAX_VERT];
int parent[MAX_VERT];

/* A structure to represent an adjacency list node */
typedef struct edgeNode *edgePtr;

typedef struct edgeNode {
    int vtx;
    int weight;
    edgePtr nxt;
} edgeNode_t;

typedef struct graph {
    int degree[MAX_VERT];
    int nvertices;
    int nEdges;
    int directed;
    edgePtr edges[MAX_VERT];
} graph_t;

void
printParent() 
{
    int i;
    for (i = 0; i < MAX_VERT; i++) {
            printf("parent of %d -> %2d\n", i, parent[i]);
    }
    printf("\n");
}

int
getCost(graph_t *G, int src, int dst)
{
    edgePtr t = G->edges[src];

    while (t) {
        if (t->vtx == dst) {
           return (t->weight);
        }
        t = t->nxt;
    }
    return MAX_COST;
}

void
printHeader(int v) 
{
    int j = 0;
    printf ("\nShortest Paths from Vertex %d\n%6c", v, ' ');
    for (j = 0; j < MAX_VERT; j++) 
        printf("  [%d]", j);
    printf("\n");
}

void
printCost(graph_t *G) 
{
    int i, j;
    printf("\n------------------Cost Matrix---------------------\n%5c", ' ');
    for (j = 0; j < G->nvertices; j++) 
        printf("  [%d]", j);
    printf("\n");

    for (i = 0; i < G->nvertices; i++) {
        printf("[%d] ", i);
        for (j = 0; j < G->nvertices; j++) {
            if (getCost(G, i, j) == MAX_COST) {
                printf("%5c", '*');
            } else {
                printf("%5d", getCost(G, i, j));
            }
        }
        printf("\n");
    }
    printf("---------------------------------------------------\n");
}

/* A utility function to print the adjacenncy list representation of graph */
void 
printGraph(graph_t *G)
{
    int v;

    for (v = 0; v < G->nvertices; ++v) {
        edgeNode_t *p = G->edges[v];
        printf("vertex[%d]", v);
        while (p) {
            printf(" -> %d", p->vtx);
            p = p->nxt;
        }
        printf("\n");
    }
}

graph_t *
createGraph(int V, int directed) 
{
    graph_t *graph = (graph_t *) malloc(sizeof(graph_t));
    graph->nvertices = V;
    graph->nEdges = 0;
    graph->directed  = directed;
    //graph->edges = (edgeNode_t *) malloc(V * sizeof(edgeNode_t));
      
    int i;
    for (i = 0; i < MAX_VERT; ++i)
       graph->degree[i]= 0;

    return graph;
}

void 
insertEdge(graph_t *G, int src, int dst, int cost, int directed)
{
    if ((src >= G->nvertices) || (dst >= G->nvertices))
        return;

    edgeNode_t *T = malloc(sizeof(edgeNode_t));
    T->weight = cost;
    T->vtx = dst;
    T->nxt = G->edges[src];

    G->edges[src] = T;
    G->degree[src]++;

    if (directed) {
        insertEdge(G, dst, src, cost, TRUE);
    } else {
        G->nEdges++;
    }
}

void
deletEdgeNode(edgeNode_t **L, int n)
{
#if 0
    edgeNode_t *tL;
    edgeNode_t *tNode; 

    while (*L && (*L)->vtx == n) {
        tNode = *L;
        *L = (*L)->nxt;
        free(tNode);
        tNode = NULL;
    }

    tL = *L;

    while (tL && tL->nxt) {
        if (tL->nxt->vtx == n) {
            tNode = tL->nxt;
            tL->nxt = tL->nxt->nxt;
            free(tNode);
        } else {
            tL = tL->nxt;
        }
    } 
#endif    
}

void
deleteEdge(graph_t *G, int src, int vtx)
{
#if 0
    edgeNode_t *L;    
    edgeNode_t *tNode;    
    
    if ((src >= G->nvertices) || (vtx >= G->nvertices))
        return;

    deletEdgeNode(&(G->edges[src].head), vtx);
    deletEdgeNode(&(G->edges[vtx].head), src);
#endif    
}

void
deleteVertex(graph_t *G, int V) 
{
#if 0    
    if (V >= G->nvertices)
        return;

    while (G->edges[V].head) {
        deleteEdge(G, V, G->edges[V].head->vtx);
    }
#endif   
}

void
clearSearch()
{
    int i;

    for (i = 0; i < MAX_VERT; i++) {
        visited[i]    = FALSE;
        discovered[i] = FALSE;
        parent[i]     = -1;
    }
}

/** Breadth First Search **********************************************************************************************/
void
BFS(graph_t *G, int v)
{
    edgeNode_t *w;
    clearSearch();
    printf ("\nBFS[%d]", v);
    queue_t *Q = createQ(MAX_VERT);
    enQueue(Q, v);
    discovered[v] = TRUE;

    while (isEmpty(Q) == 0) {
        v = deQueue(Q);
        printf("->%2d", v);
        visited[v] = TRUE;

        for (w = G->edges[v]; w; w = w->nxt) {
      //      if (visited[w->vtx] == FALSE) {
      //          printf("->%2d", w->vtx);
      //      }
            if (discovered[w->vtx] == FALSE) {
                enQueue(Q, w->vtx);
                discovered[w->vtx] = TRUE;
                parent[w->vtx] = v;
            }
        }
    }
    printf("\n");
}

/** Depth First Search ************************************************************************************************/
void
DFS(graph_t *G, int V) 
{
    edgeNode_t *w;
    visited[V] = 1;
    printf("->%2d", V);
    for (w = G->edges[V]; w; w = w->nxt) {
        if (!visited[w->vtx]) {
            DFS(G, w->vtx);
        }
    }
}

/** Find path between two vertices ************************************************************************************/
void
_findPath(int start, int end)
{
    if ((start == end) || (end == -1)) {
        printf("->%2d", start);
    } else {
        _findPath(start, parent[end]);
        printf("->%2d", end);
    }
}

void
findPath(graph_t *G, int s, int e)
{
    clearSearch();
    BFS(G, s);
    printf("path between %d and %d is : ", s, e);
    _findPath(s, e);
    printf("\n");
}

/** Connected component **********************************************************************************************
    Connected component of undirected graph is a maximal set of vertices such that
    there is a path between every pair of vertices.
    This is easily accomplished by  making repeated calls to either dfs(v) or bfs(v) 
    where v is unvisited vertex
*/
void
connectedComponent(graph_t *G)
{
    int i;
    printf("connectedComponent\n");
    clearSearch();
    for (i = 0; i < MAX_VERT; i++) {
        if (!discovered[i]) {
           BFS(G, i);
        }
    }
}

/** Spanning Tree ******************************************************************************************************
    Spanning Tree of a graph G = (V, E) is a subset of edges from E forming a tree connecting all vertices of V.
    Spanning tree can be found using DFS or BFS search.
    Minimum Spanning Tree
        A minimum spanning tree minimize the total length over all possible spanning trees.
        There can be more than one minimum spanning trees in a graph.
        All spanning trees of unweighted graph G are minimum spanning trees
        Finding minimum spanning tree for weighted graph is more difficult, Algorithms below
        demonstrate the optimality of certain greedy heuristics
           1. Kruskal's algorithm
           2. Prim's Algorithm
           3. Sollin's Algorithm
*/

/** Kruskal's Algorithm **********************************************************************************************/
typedef struct setUnion {
    int p[MAX_VERT];
    int size[MAX_VERT];
    int n;
} setUnion_t;

typedef struct edgePair {
    int x;
    int y;
    int weight;
} edgePair_t;

void
setUnionInit(setUnion_t *s, int n)
{
    int i;

    for (i = 0; i < n; i++) {
         s->p[i] = i;
         s->size[i] = 1;
    }
    s->n = n;
}

printUnionSet(setUnion_t *s)
{
    int i; 
    for (i = 0; i < s->n; i++) {
        printf(" p[%d] = %d (%d)", i, s->p[i], s->size[i]);
    }
    printf("\n");
}

int
find(setUnion_t *s, int x)
{
     if (s->p[x] == x)
         return (x);
     else
         return (find(s, s->p[x]));
}

int
unionSets(setUnion_t *s, int s1, int s2)
{
    int r1, r2;
    r1 = find(s, s1);
    r2 = find(s, s2);

    if (r1 == r2)
        return;

    if (s->size[r1] >= s->size[r2]) {
        s->size[r1] = s->size[r1] + s->size[r2];
        s->p[r2] = r1;
    } else {
        s->size[r2] = s->size[r1] + s->size[r2];
        s->p[r1] = r2;
    }
}

int
sameComponent(setUnion_t *s, int s1, int s2)
{
    return (find(s, s1) == find(s, s2));
}

void
initEdgeArray(graph_t *G, edgePair_t *e)
{
     int i;

     for (i = 0; i < G->nEdges; i++) {
          e[i].x = 0;
          e[i].y = 0;
          e[i].weight = 0;
     }
}

void
getEdgeArray(graph_t *G, edgePair_t *e)
{
     int i;
     int k = 0;
     edgePtr edge = NULL;

     initEdgeArray(G, e);

     for (i = 0; i < G->nvertices; i++) {
         edge = G->edges[i];
         while(edge) {
              e[k].x = i;
              e[k].y = edge->vtx;
              e[k].weight = edge->weight;
              k++;
              edge = edge->nxt;
         }
     }
}

void
printEdgeArray(graph_t *G, edgePair_t *e)
{
     int i;

     for (i = 0; i < G->nEdges; i++) {
         printf("edgeArray[%d], edge from %d to %d weight %d\n", i, e[i].x, e[i].y, e[i].weight);
     }
}

void 
swap (edgePair_t *E, int a, int b)
{
    edgePair_t t;

    t = E[a];
    E[a] = E[b];
    E[b] = t;
}

void
Sort(graph_t *G, edgePair_t *A) 
{
    int i, j; // Pass
    for (i = 0; i < (G->nEdges - 1);  i++) {
        for (j = 0; j < (G->nEdges - i - 1); j++) {
            if (A[j].weight > A[j+1].weight) {
                swap(A, j, j+1);
            }
        }
    }
}

kruskal(graph_t *G)
{
    int i;
    setUnion_t s;
    edgePair_t e[MAX_VERT*MAX_VERT];
    int weightComapare();

    setUnionInit(&s, G->nvertices);

    getEdgeArray(G, e);
    Sort(G, e);
    for (i = 0; i < G->nEdges; i++) {
        if (!sameComponent(&s, e[i].x, e[i].y)) {
            printf("edge (%d, %d) in MST\n", e[i].x, e[i].y);
            unionSets(&s, e[i].x, e[i].y);
        }
    }
}

/** Shortest Path for weighted graph  Dikjstra's Algorithm ***********************************************************/
void
printDistance() 
{
    int i;
    for (i = 0; i < MAX_VERT; i++) {
        if (distance[i] == MAX_COST) {
            printf("%5c", '*');
        } else {
            printf("%5d", distance[i]);
        }
    }
    printf("\n");
}

int
choose (int *distance, int n, int *found)
{
    int i, min, minpos;
    min = 9999;
    minpos = -1;

    for (i = 0; i < MAX_VERT; i++) {
        if (distance[i] < min && !found[i]) {
            min = distance[i];
            minpos = i;
        }
    }

    found[minpos] = TRUE;
    printf("[%d]   ", minpos);  /* Print u */
    return minpos;
}

/* Dikjastra's algorithm */
void
shortestPath(graph_t *G, int v, int *distance, int *found)
{
    int i, u, w;
    int n  = MAX_VERT; 

    for (i =0; i < n; i++) {
        found[i] = FALSE;
        distance[i] = getCost(G, v, i);
    }

    found[v] = TRUE;
    distance[v] = 0;

    printf("[%c]   ", '*');
    printDistance();

    for (i = 0; i < (n - 2); i++) {
        u = choose(distance, n, found);
        for (w = 0; w < n; w++) {
            if (!found[w]) {
                 /* If cost of reaching vertext w from u is less than known cost of reaching w , 
                    update distance[w] with new cost to reach w */
                if ((distance[u] + getCost(G, u, w)) < distance[w]) {
                    distance[w] = distance[u] + getCost(G, u, w);  
                }
            }
        } /* end of w-loop */
        printDistance();
    } /* end of i-loop */
}

/** Floyd's All Pairs Shortest Path for weighted graph Algorithm ***********************************************************/

int W[MAX_VERT][MAX_VERT] = {99};

getWeightMatrixFrmG(graph_t *G)
{
    int i, j;
    edgePair_t E[MAX_VERT * MAX_VERT];
    getEdgeArray(G, E);

    for (i = 0; i < MAX_VERT;  i++) {
        for (j = 0; j < MAX_VERT;  j++) {
            W[i][j] = 99;
        }
    }

    for (i = 0; i < G->nEdges;  i++) {
         if (E[i].weight == 0) {
             W[E[i].x][E[i].y] = 99;
         } else {
             W[E[i].x][E[i].y] = E[i].weight;
         }
    }
}

printWeightArray(graph_t *G, int W[MAX_VERT][MAX_VERT])
{
    int i, j;

    for (i = 0; i < G->nvertices;  i++) {
        for (j = 0; j < G->nvertices;  j++) {
            if (W[i][j] == 99) {
                printf("%3c", '*');
            } else {    
                printf("%3d", (W[i][j]));
            }
        }
        printf ("\n");
    }
    printf ("\n");
}

flyod()
{
    int i, j, k;

    for (k = 0; k < MAX_VERT;  k++) {
        for (i = 0; i < MAX_VERT;  i++) {
            for (j = 0; j < MAX_VERT;  j++) {
               if ((W[i][k] + W[k][j]) < W[i][j])
                   W[i][j] = W[i][k] + W[k][j];
            }
        }
    }
}

int
main (void)
{
    int V = 8;
    graph_t *G = createGraph(V, FALSE);

    edgePair_t e[MAX_VERT*MAX_VERT];
    graph_t *G1;

    insertEdge(G, 1, 0,  300, 0);
    insertEdge(G, 2, 0, 1000, 0);
    insertEdge(G, 2, 1,  800, 0);
    insertEdge(G, 3, 2, 1200, 0);
    insertEdge(G, 4, 3, 1500, 0);
    insertEdge(G, 4, 5,  250, 0);
    insertEdge(G, 5, 3, 1000, 0);
    insertEdge(G, 5, 6,  900, 0);
    insertEdge(G, 5, 7, 1400, 0);
    insertEdge(G, 6, 7, 1000, 0);
    insertEdge(G, 7, 0, 1700, 0);

    /* print the adjacency list representation of the above graph */
    printGraph(G);

    printf ("\nDFS[4]");
    DFS(G, 4);
    BFS(G, 4);
    printf("\n");
    printParent();
    printCost(G);

    /* Shortest path */
    V = 4;
    printHeader(V);
    shortestPath(G, V, distance, found);

    findPath(G, 4, 0);
    connectedComponent(G);

    V = 7;
    G1 = createGraph(V, FALSE);
   
    insertEdge(G1, 0, 1, 28, 0);
    insertEdge(G1, 0, 5, 10, 0);
    insertEdge(G1, 1, 2, 16, 0);
    insertEdge(G1, 1, 6, 14, 0);
    insertEdge(G1, 2, 3, 12, 0);
    insertEdge(G1, 3, 4, 22, 0);
    insertEdge(G1, 4, 5, 25, 0);
    insertEdge(G1, 6, 4, 24, 0);
    insertEdge(G1, 6, 3, 18, 0);

    printGraph(G1);
    printCost(G1);

    kruskal(G1);

    printf("\nKruskal on G\n");
    kruskal(G);
    fprintf(stderr, "END\n");

    getWeightMatrixFrmG(G1);

    printf("Flyod's all path cost\n");
    printWeightArray(G1, W);
    flyod();
    printWeightArray(G1, W);
    return 0;
}
