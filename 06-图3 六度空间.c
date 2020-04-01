#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 1001

/* 无向图的邻接表定义——开始 */
typedef int Vertex;
typedef struct AdjVNode *PtrToAdjVNode;
struct AdjVNode {
    Vertex AdjV;
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
};
typedef PtrToENode Edge;

LGraph CreateGraph( int VertexNum )
{
    Vertex V;
    LGraph Graph;

    Graph = (LGraph)malloc(sizeof(struct GNode));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;

    for (V = 1; V <= Graph->Nv; ++V)
        Graph->G[V].FirstEdge = NULL;

    return Graph;
}

void DestoryGraph( LGraph Graph )
{
    Vertex V;
    PtrToAdjVNode Node;
    for (V = 1; V <= Graph->Nv; ++V) {
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
    NewNode->AdjV = E->V2;
    NewNode->Next = Graph->G[E->V1].FirstEdge;
    Graph->G[E->V1].FirstEdge = NewNode;

    NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    NewNode->AdjV = E->V1;
    NewNode->Next = Graph->G[E->V2].FirstEdge;
    Graph->G[E->V2].FirstEdge = NewNode;
}
/* 无向图的邻接表定义——结束 */

/* 队列定义开始 */
#define MaxSize 1001
#define ERROR -1
typedef int Position;
struct QNode {
    int *Data;     /* 存储元素的数组 */
    Position Front, Rear;  /* 队列的头、尾指针 */
};
typedef struct QNode *Queue;

Queue CreateQueue()
{
    Queue Q = (Queue)malloc(sizeof(struct QNode));
    Q->Data = (int *)malloc(MaxSize * sizeof(int));
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

void Enqueue( Queue Q, int X )
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

int Dequeue( Queue Q )
{
    if ( IsEmpty(Q) ) return ERROR;
    else  {
        Q->Front =(Q->Front+1)%MaxSize;
        return  Q->Data[Q->Front];
    }
}
/* 队列定义结束 */
Vertex visited[MaxVertexNum] = {0};
void setZero(int VertexNum)
{
    Vertex V;
    for (V = 1; V <= VertexNum; ++V)
        visited[V] = 0;
}

LGraph BuildGraph()
{
    LGraph Graph;
    Edge E;
    int Nv, i;

    scanf("%d", &Nv);
    Graph = CreateGraph(Nv);
    scanf("%d", &(Graph->Ne));
    if (Graph->Ne != 0) {
        E = (Edge)malloc(sizeof(struct ENode));
        for (i = 0; i < Graph->Ne; ++i) {
            scanf("%d %d", &E->V1, &E->V2);
            InsertEdge(Graph, E);
        }
        free(E);
    }
    return Graph;
}

int BFS(LGraph Graph, Vertex V)
{
    int count, level;
    Vertex tail, last; Queue Q;
    PtrToAdjVNode Node;
    Q = CreateQueue();
    setZero(Graph->Nv);

    visited[V] = 1; count = 1;
    level = 0; last = V; tail = V;
    Enqueue(Q, V);
    while(!IsEmpty(Q)) {
        V = Dequeue(Q);
        for (Node = Graph->G[V].FirstEdge; Node; Node = Node->Next) {
            if (!visited[Node->AdjV]) {
                visited[Node->AdjV] = 1;
                Enqueue(Q, Node->AdjV); ++count;
                tail = Node->AdjV;
            }
        }
        if (V == last) {
            ++level; last = tail;
        }
        if (level == 6) break;
    }

    DestoryQueue(Q);
    return count;
}

void SDS(LGraph Graph)
{
    Vertex V;
    double count;
    for (V = 1; V <= Graph->Nv; ++V) {
        count = BFS(Graph, V);
        printf("%d: %.2f%%\n", V, count / Graph->Nv * 100);
    }
}

int main()
{
    LGraph Graph;
    Graph = BuildGraph();
    SDS(Graph);
    DestoryGraph(Graph);

    return 0;
}
