#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 500
#define INFINITY 65535
/* 无向图的邻接表定义——开始 */
typedef int Vertex;
typedef struct AdjVNode *PtrToAdjVNode;
struct AdjVNode {
    Vertex AdjV;
    int length, cost;
    PtrToAdjVNode Next;
};

typedef struct Vnode {
    PtrToAdjVNode FirstEdge;
} AdjList[MaxVertexNum];

typedef struct GNode *PtrToGNode;
struct GNode {
    int Nv;
    int Ne;
    AdjList G;
};
typedef PtrToGNode LGraph;

typedef struct ENode *PtrToENode;
struct ENode {
    Vertex V1, V2;
    int length, cost;
};
typedef PtrToENode Edge;

LGraph CreateGraph( int VertexNum )
{
    Vertex V;
    LGraph Graph;

    Graph = (LGraph)malloc(sizeof(struct GNode));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;

    for (V = 0; V < Graph->Nv; ++V)
        Graph->G[V].FirstEdge = NULL;

    return Graph;
}

void DestoryGraph( LGraph Graph )
{
    Vertex V;
    PtrToAdjVNode Node;
    for (V = 0; V < Graph->Nv; ++V) {
        while (Graph->G[V].FirstEdge) {
            Node = Graph->G[V].FirstEdge;
            Graph->G[V].FirstEdge = Node->Next;
            free(Node);
        }
    }
    free(Graph);
}

void InsertEdge(LGraph Graph, Edge E)
{
    PtrToAdjVNode NewNode;

    NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    NewNode->AdjV = E->V2; NewNode->length = E->length; NewNode->cost = E->cost;
    NewNode->Next = Graph->G[E->V1].FirstEdge;
    Graph->G[E->V1].FirstEdge = NewNode;
}
/* 无向图的邻接表定义——结束 */
#define ERROR -1
int source, destination;
int collected[MaxVertexNum], dist[MaxVertexNum], cost[MaxVertexNum];

LGraph BuildGraph();
void init(LGraph Graph);
int FindMinDist(LGraph Graph);
void solve(LGraph Graph);

int main()
{
    LGraph Graph;
    Graph = BuildGraph();
    solve(Graph);
    DestoryGraph(Graph);

    return 0;
}

LGraph BuildGraph()
{
    int Nv, i; Edge E;
    Vertex V; LGraph Graph;

    scanf("%d", &Nv);
    Graph = CreateGraph(Nv);
    scanf("%d", &(Graph->Ne));
    scanf("%d %d", &source, &destination);
    if (Graph->Ne != 0) {
        E = (Edge)malloc(sizeof(struct ENode));
        for (i = 0; i < Graph->Ne; ++i) {
            scanf("%d %d %d %d", &E->V1, &E->V2, &E->length, &E->cost);
            InsertEdge(Graph, E);
            V = E->V1; E->V1 = E->V2; E->V2 = V;
            InsertEdge(Graph, E);
        }
        free(E);
    }
    return Graph;
}

void init(LGraph Graph)
{
    Vertex V;
    PtrToAdjVNode edge;
    for (V = 0; V < Graph->Nv; ++V) {
        collected[V] = 0;
        dist[V] = INFINITY;
        cost[V] = INFINITY;
    }
    for (edge = Graph->G[source].FirstEdge; edge; edge = edge->Next) {
        dist[edge->AdjV] = edge->length;
        cost[edge->AdjV] = edge->cost;
    }
}

int FindMinDist(LGraph Graph)
{
    Vertex MinV, V;
    int MinDist = INFINITY;

    for (V = 0; V < Graph->Nv; ++V) {
        if (collected[V] == 0  && dist[V] < MinDist) {
            MinDist = dist[V];
            MinV = V;
        }
    }
    if (MinDist < INFINITY)
        return MinV;
    else return ERROR;
}

void solve(LGraph Graph)
{
    PtrToAdjVNode edge;
    Vertex V;

    init(Graph);
    collected[source] = 1; dist[source] = 0; cost[source] = 0;
    while (1) {
        V = FindMinDist(Graph);
        if (V == ERROR)
            break;
        collected[V] = 1;
        for (edge = Graph->G[V].FirstEdge; edge; edge = edge->Next) {
            if (collected[edge->AdjV] == 0 &&
                (dist[V] + edge->length < dist[edge->AdjV] ||
                 (dist[V] + edge->length == dist[edge->AdjV] && cost[V] + edge->cost < cost[edge->AdjV]))) {
                dist[edge->AdjV] = dist[V] + edge->length;
                cost[edge->AdjV] = cost[V] + edge->cost;
            }
        }
    }
    printf("%d %d\n", dist[destination], cost[destination]);
}
