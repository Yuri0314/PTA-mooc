#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 100
/*-------------------- 图的邻接表定义 --------------------*/
typedef int Vertex;
typedef int WeightType;
typedef struct AdjVNode *PtrToAdjVNode;
struct AdjVNode {
    Vertex AdjV;
    WeightType time;
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
    WeightType time;
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
    NewNode->AdjV = E->V2; NewNode->time = E->time;
    NewNode->Next = Graph->G[E->V1].FirstEdge;
    Graph->G[E->V1].FirstEdge = NewNode;
}
/*-------------------- 邻接表定义结束 --------------------*/
/*-------------------- 队列定义 --------------------*/
#define MaxSize 101
#define ERROR -1
typedef int Position;
typedef Vertex ElementType;
struct QNode {
    ElementType *Data;     /* 存储元素的数组 */
    Position Front, Rear;  /* 队列的头、尾指针 */
};
typedef struct QNode *Queue;

Queue CreateQueue()
{
    Queue Q = (Queue)malloc(sizeof(struct QNode));
    Q->Data = (ElementType *)malloc(MaxSize * sizeof(ElementType));
    Q->Front = Q->Rear = 0;
    return Q;
}

void DestoryQueue( Queue Q )
{
    if (Q->Data) free(Q->Data);
    free(Q);
}

int IsFull( Queue Q )
{
    return ((Q->Rear+1)%MaxSize == Q->Front);
}

void Enqueue( Queue Q, ElementType X )
{
    if ( IsFull(Q) ) return;
    else {
        Q->Rear = (Q->Rear+1)%MaxSize;
        Q->Data[Q->Rear] = X;
    }
}

int IsEmpty( Queue Q )
{
    return (Q->Front == Q->Rear);
}

ElementType Dequeue( Queue Q )
{
    if ( IsEmpty(Q) ) return ERROR;
    else  {
        Q->Front =(Q->Front+1)%MaxSize;
        return  Q->Data[Q->Front];
    }
}
/*-------------------- 队列定义结束 --------------------*/
int inDegree[MaxVertexNum];
WeightType completionTime[MaxVertexNum];

LGraph buildGraph();
WeightType TopSort(LGraph graph);   // 拓扑排序

int main()
{
    LGraph graph;
    WeightType ans;
    graph = buildGraph();
    ans = TopSort(graph);
    if (ans != -1)
        printf("%d", ans);
    else
        printf("Impossible");
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
        E = (Edge)malloc(sizeof(struct ENode));
        for (i = 0; i < graph->Ne; ++i) {
            scanf("%d %d %d", &E->V1, &E->V2, &E->time);
            InsertEdge(graph, E);
            ++inDegree[E->V2];
        }
        free(E);
    }
    return graph;
}

WeightType TopSort(LGraph graph)
{
    Queue Q;
    Vertex v;
    WeightType finalTime;
    int cnt;
    PtrToAdjVNode graphEdge;

    Q = CreateQueue();
    for (v = 0; v < graph->Nv; ++v) {
        if (!inDegree[v]) {
            Enqueue(Q, v);
            completionTime[v] = 0;  // 初始化起始结点的completion time
        }
    }
    cnt = 0; finalTime = -1;
    while (!IsEmpty(Q)) {
        v = Dequeue(Q);
        ++cnt;
        for (graphEdge = graph->G[v].FirstEdge; graphEdge; graphEdge = graphEdge->Next) {
            if (completionTime[v] + graphEdge->time > completionTime[graphEdge->AdjV])
                completionTime[graphEdge->AdjV] = completionTime[v] + graphEdge->time;
            if (--inDegree[graphEdge->AdjV] == 0)
                Enqueue(Q, graphEdge->AdjV);
            if (completionTime[graphEdge->AdjV] > finalTime)    // 最大的completion time即为最终结果
                finalTime = completionTime[graphEdge->AdjV];
        }
    }
    DestoryQueue(Q);
    if (cnt != graph->Nv)
        finalTime = -1;
    return finalTime;
}
