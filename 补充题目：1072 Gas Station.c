#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 65535
#define ERROR -1
/* ——————无向图的邻接表定义开始—————— */
#define MaxVertexNum 1010
typedef int Vertex;
typedef int WeightType;

typedef struct ENode *PtrToENode;
struct ENode{
    char vertex1[4], vertex2[4];
    WeightType dist;  // 路的长度
};
typedef PtrToENode Edge;

typedef struct AdjVNode *PtrToAdjVNode;
struct AdjVNode{
    Vertex AdjV;
    WeightType dist;  // 路的长度
    PtrToAdjVNode Next;
};

typedef struct Vnode{
    PtrToAdjVNode FirstEdge;
} AdjList[MaxVertexNum];

typedef struct GNode *PtrToGNode;
struct GNode{
    int HouseNum;   // the total number of houses
    /* Candidate存储在house结点后面 */
    int Candidate;     // the total number of the candidate locations for the gas stations
    int Ne;
    AdjList G;
};
typedef PtrToGNode LGraph;

LGraph CreateGraph( int HouseNum, int Candidate );
void DestoryGraph( LGraph Graph );
void InsertEdge(LGraph Graph, Edge E);
/* ——————无向图的邻接表定义结束—————— */

int serviceRange;
int solutionIndex;
float minimumDis, averageDis;
bool collected[MaxVertexNum];
WeightType dist[MaxVertexNum];

LGraph BuildGraph();
void init(LGraph Graph);
int FindMinDist(LGraph Graph);
bool checkCandidate(LGraph Graph, int candidateIndex);
void solve(LGraph Graph);

int main()
{
    LGraph graph;
    graph = BuildGraph();
    solve(graph);
    DestoryGraph(graph);
    return 0;
}

LGraph CreateGraph( int HouseNum, int Candidate )
{
    Vertex V;
    LGraph Graph;

    Graph = (LGraph)malloc(sizeof(struct GNode));
    Graph->HouseNum = HouseNum;
    Graph->Candidate = Candidate;
    Graph->Ne = 0;

    for (V = 0; V < (Graph->HouseNum + Graph->Candidate); ++V)
        Graph->G[V].FirstEdge = NULL;

    return Graph;
}

void DestoryGraph( LGraph Graph )
{
    Vertex V;
    PtrToAdjVNode Node;
    for (V = 0; V < (Graph->HouseNum + Graph->Candidate); ++V) {
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
    Vertex V1, V2;

    V1 = (E->vertex1[0] == 'G') ? (atoi(E->vertex1 + 1) + Graph->HouseNum) : atoi(E->vertex1);
    V2 = (E->vertex2[0] == 'G') ? (atoi(E->vertex2 + 1) + Graph->HouseNum) : atoi(E->vertex2);
    --V1; --V2; // 编号都是从1开始的
    // 下面这一步必须判断，题目给的数据并没有遵守题意，给出的城市编号有比N大的值，对应最后一个测试点“段错误”，巨坑！！
    if (((E->vertex1[0] != 'G') && atoi(E->vertex1) > Graph->HouseNum) ||
        ((E->vertex2[0] != 'G') && atoi(E->vertex2) > Graph->HouseNum))
        return;

    NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    NewNode->AdjV = V2; NewNode->dist = E->dist;
    NewNode->Next = Graph->G[V1].FirstEdge;
    Graph->G[V1].FirstEdge = NewNode;

    NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    NewNode->AdjV = V1; NewNode->dist = E->dist;
    NewNode->Next = Graph->G[V2].FirstEdge;
    Graph->G[V2].FirstEdge = NewNode;
}

LGraph BuildGraph()
{
    LGraph graph;
    Edge E;
    int HouseNum, Candidate, i;

    scanf("%d %d", &HouseNum, &Candidate);
    graph = CreateGraph(HouseNum, Candidate);
    scanf("%d", &(graph->Ne));
    scanf("%d", &serviceRange);
    if (graph->Ne != 0) {
        E = (Edge)malloc(sizeof(struct ENode));
        for (i = 0; i < graph->Ne; ++i) {
            scanf("%s %s %d", E->vertex1, E->vertex2, &(E->dist));
            InsertEdge(graph, E);
        }
        free(E);
    }

    return graph;
}

void init(LGraph Graph)
{
    Vertex V;
    for (V = 0; V < (Graph->HouseNum + Graph->Candidate); ++V) {
        collected[V] = false;
        dist[V] = INF;
    }
}

int FindMinDist(LGraph Graph)
{
    Vertex MinV, V;
    int MinDist = INF;

    for (V = 0; V < (Graph->HouseNum + Graph->Candidate); ++V) {
        if (!collected[V] && dist[V] < MinDist) {
            MinDist = dist[V];
            MinV = V;
        }
    }
    if (MinDist < INF) return MinV;
    else return ERROR;
}

bool checkCandidate(LGraph Graph, int candidateIndex)
{
    Vertex source, V;
    PtrToAdjVNode edge;
    double sum, minDis, average;

    source = Graph->HouseNum + candidateIndex;
    if (!Graph->G[source].FirstEdge) return false;
    init(Graph);
    // 先将源点放入已取元素的集合中，然后更改其邻接点相关值
    collected[source] = true; dist[source] = 0;
    for (edge = Graph->G[source].FirstEdge; edge; edge = edge->Next) {
        if (edge->dist < dist[edge->AdjV])
            dist[edge->AdjV] = edge->dist;
    }
    // 然后依次从未取元素中找距离最小的元素放入集合中
    while (true) {
        V = FindMinDist(Graph);
        if (dist[V] > serviceRange && V < Graph->HouseNum)
            return false;
        if (V == ERROR)
            break;
        collected[V] = true;
        for (edge = Graph->G[V].FirstEdge; edge; edge = edge->Next) {
            if (!collected[edge->AdjV] && (dist[V] + edge->dist < dist[edge->AdjV])) {
                dist[edge->AdjV] = dist[V] + edge->dist;
            }
        }
    }
    minDis = INF; sum = 0;
    for (V = 0; V < Graph->HouseNum; ++V) {
        if (dist[V] == INF) return false;
        if (dist[V] < minDis)
            minDis = dist[V];
        sum += dist[V];
    }
    average = sum / Graph->HouseNum;
    if (minDis > minimumDis || (minDis == minimumDis && average < averageDis)) {
        solutionIndex = candidateIndex + 1;
        minimumDis = minDis;
        averageDis = average;
    }
    return true;
}

void solve(LGraph Graph)
{
    int i;
    bool flag = false;
    minimumDis = 0;
    averageDis = INF;
    for (i = 0; i < Graph->Candidate; ++i) {
        if (checkCandidate(Graph, i))
            flag = true;
    }
    if (flag) printf("G%d\n%.1lf %.1lf\n", solutionIndex, minimumDis, averageDis);
    else printf("No Solution\n");
}
