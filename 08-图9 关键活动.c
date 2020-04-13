#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 100
#define INFINITY 65535
/*-------------------- 图的邻接表定义 --------------------*/
typedef int Vertex;
typedef int WeightType;
typedef struct ENode *PtrToENode;
struct ENode {
    Vertex V1, V2;
    WeightType time;
    PtrToENode NextPointTo;
    PtrToENode NextBePointTo;
};
typedef PtrToENode Edge;

typedef struct Vnode {
    WeightType earliest, latest;    // 每个结点最早和最晚完成时间
    PtrToENode FirstPointToEdge; // 该结点发出的边链表
    PtrToENode FirstBePointToEdge;   // 指向该结点的边链表
} AdjList[MaxVertexNum];

typedef struct GNode *PtrToGNode;
struct GNode {
    int Nv;
    int Ne;
    AdjList G;
};
typedef PtrToGNode LGraph;

LGraph CreateGraph( int VertexNum )
{
    Vertex V;
    LGraph Graph;

    Graph = (LGraph)malloc(sizeof(struct GNode));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;

    for (V = 0; V < Graph->Nv; ++V) {
        Graph->G[V].earliest = 0;
        Graph->G[V].latest = INFINITY;
        Graph->G[V].FirstPointToEdge = NULL;
        Graph->G[V].FirstBePointToEdge = NULL;
    }

    return Graph;
}

void DestoryGraph( LGraph Graph )
{
    Vertex V;
    PtrToENode Node;
    for (V = 0; V < Graph->Nv; ++V) {
        while (Graph->G[V].FirstPointToEdge) {
            Node = Graph->G[V].FirstPointToEdge;
            Graph->G[V].FirstPointToEdge = Node->NextPointTo;
            free(Node);
        }
    }
    free(Graph);
}

void InsertEdge(LGraph Graph, Edge E)
{
    E->NextPointTo = Graph->G[E->V1].FirstPointToEdge;
    Graph->G[E->V1].FirstPointToEdge = E;
    E->NextBePointTo = Graph->G[E->V2].FirstBePointToEdge;
    Graph->G[E->V2].FirstBePointToEdge = E;
}
/*-------------------- 邻接表定义结束 --------------------*/
int container[MaxVertexNum];
int inDegree[MaxVertexNum];

LGraph buildGraph();
void solve(LGraph graph);
int forward(LGraph graph);  // 正向遍历
void backward(LGraph graph, WeightType finalTime);  // 反向遍历
void printPath(LGraph graph);   // 打印关键路径

int main()
{
    LGraph graph;
    graph = buildGraph();
    solve(graph);
    DestoryGraph(graph);

    return 0;
}

LGraph buildGraph()
{
    LGraph graph; Edge E;
    int vertexNum, i;

    scanf("%d", &vertexNum);
    graph = CreateGraph(vertexNum);
    scanf("%d", &(graph->Ne));
    if (graph->Ne != 0) {
        for (i = 0; i < graph->Ne; ++i) {
            E = (Edge)malloc(sizeof(struct ENode));
            scanf("%d %d %d", &E->V1, &E->V2, &E->time);
            --E->V1; --E->V2;   // 注意输入点的编号与存储索引差1
            InsertEdge(graph, E);
            ++inDegree[E->V2];
        }
    }
    return graph;
}

void solve(LGraph graph)
{
    WeightType ans;

    ans = forward(graph);
    if (ans == -1) printf("0");
    else {
        printf("%d\n", ans);
        backward(graph, ans);
        printPath(graph);
    }
}

int forward(LGraph graph)
{
    Vertex v;
    WeightType finalTime;
    int front, rear, cnt;
    Edge edge;

    front = rear = -1;  // 队列的头尾
    for (v = 0; v < graph->Nv; ++v) {
        if (!inDegree[v]) {
            container[++rear] = v;
            graph->G[v].earliest = 0;  // 初始化起始结点的earliest
        }
    }
    cnt = 0; finalTime = -1;
    while (front != rear) {
        v = container[++front];
        ++cnt;
        for (edge = graph->G[v].FirstPointToEdge; edge; edge = edge->NextPointTo) {
            if (graph->G[v].earliest + edge->time > graph->G[edge->V2].earliest)
                graph->G[edge->V2].earliest = graph->G[v].earliest + edge->time;
            if (--inDegree[edge->V2] == 0)
                container[++rear] = edge->V2;
            if (graph->G[edge->V2].earliest > finalTime)    // 最大的earliest即为最终结果
                finalTime = graph->G[edge->V2].earliest;
        }
    }
    if (cnt != graph->Nv)
        return -1;
    return finalTime;
}

void backward(LGraph graph, WeightType finalTime)
{
    Vertex v;
    int i;
    Edge edge;

    for (i = graph->Nv; i > 0;) {
        v = container[--i];
        // 将网络最后出度为0的结点的latest值置为earliest值并且该结点earliest必须等于finalTime
        if (!graph->G[v].FirstPointToEdge && graph->G[v].earliest == finalTime)
            graph->G[v].latest = graph->G[v].earliest;
        for (edge = graph->G[v].FirstBePointToEdge; edge; edge = edge->NextBePointTo) {
            if (graph->G[v].latest - edge->time < graph->G[edge->V1].latest)
                graph->G[edge->V1].latest = graph->G[v].latest - edge->time;
        }
    }
}

void printPath(LGraph graph)
{
    Vertex v;
    Edge edge;

    for (v = 0; v < graph->Nv; ++v) {
        for (edge = graph->G[v].FirstPointToEdge; edge; edge = edge->NextPointTo) {
            if (graph->G[edge->V2].latest - graph->G[v].earliest == edge->time)
                printf("%d->%d\n", (v + 1), (edge->V2 + 1));
        }
    }
}
