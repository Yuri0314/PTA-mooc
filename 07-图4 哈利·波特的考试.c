#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 100
#define INFINITY 65535
typedef int Vertex;
typedef int WeightType;

typedef struct ENode *PtrToENode;
struct ENode {
    Vertex V1, V2;
    WeightType Weight;
};
typedef PtrToENode Edge;

typedef struct GNode *PtrToGNode;
struct GNode {
    int Nv;
    int Ne;
    WeightType G[MaxVertexNum][MaxVertexNum];
};
typedef PtrToGNode MGraph;

MGraph CreateGraph(int);
void InsertEdge(MGraph, Edge);
MGraph BuildGraph();
void DestoryGraph(MGraph);

void Floyd(MGraph, WeightType D[][MaxVertexNum]);
WeightType FindMaxDist(WeightType D[][MaxVertexNum], Vertex, int);
void FindAnimal(MGraph);

int main()
{
    MGraph G = BuildGraph();
    FindAnimal(G);

    return 0;
}

MGraph CreateGraph(int VertexNum)
{
    Vertex V, W;
    MGraph Graph;

    Graph = (MGraph)malloc(sizeof(struct GNode));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;

    for (V = 0; V < Graph->Nv; ++V)
        for (W = 0; W < Graph->Nv; ++W)
            Graph->G[V][W] = INFINITY;

    return Graph;
}

void InsertEdge(MGraph Graph, Edge E)
{
    Graph->G[E->V1][E->V2] = E->Weight;
    Graph->G[E->V2][E->V1] = E->Weight;
}

MGraph BuildGraph()
{
    MGraph Graph;
    Edge E;
    int Nv, i;

    scanf("%d", &Nv);
    Graph = CreateGraph(Nv);

    scanf("%d", &(Graph->Ne));
    if (Graph->Ne != 0) {
        E = (Edge)malloc(sizeof(struct ENode));
        for (i = 0; i < Graph->Ne; ++i) {
            scanf("%d %d %d", &E->V1, &E->V2, &E->Weight);
            --E->V1; --E->V2;
            InsertEdge(Graph, E);
        }
        free(E);
    }

    return Graph;
}

void DestoryGraph(MGraph Graph)
{
    if (Graph) free(Graph);
}

void Floyd(MGraph Graph, WeightType D[][MaxVertexNum])
{
    Vertex i, j, k;

    for (i = 0; i < Graph->Nv; ++i)
        for (j = 0; j < Graph->Nv; ++j)
            D[i][j] = Graph->G[i][j];

    for (k = 0; k < Graph->Nv; ++k)
        for (i = 0; i < Graph->Nv; ++i)
            for (j = 0; j < Graph->Nv; ++j) {
                if (D[i][k] + D[k][j] < D[i][j])
                    D[i][j] = D[i][k] + D[k][j];
            }
}

WeightType FindMaxDist(WeightType D[][MaxVertexNum],
                       Vertex i, int N)
{
    WeightType MaxDist;
    Vertex j;

    MaxDist = 0;
    for (j = 0; j < N; ++j)
        if (i != j && D[i][j] > MaxDist)
            MaxDist = D[i][j];

    return MaxDist;
}

void FindAnimal(MGraph Graph)
{
    WeightType D[MaxVertexNum][MaxVertexNum], MaxDist, MinDist;
    Vertex Animal, i;

    Floyd(Graph, D);

    MinDist = INFINITY;
    for (i = 0; i < Graph->Nv; ++i) {
        MaxDist = FindMaxDist(D, i, Graph->Nv);
        if (MaxDist == INFINITY) {
            printf("0\n");
            return;
        }
        if (MinDist > MaxDist) {
            MinDist = MaxDist; Animal = i + 1;
        }
    }
    printf("%d %d\n", Animal, MinDist);
}
