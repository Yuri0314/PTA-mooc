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
    AdjList G;
};
typedef PtrToGNode LGraph;

LGraph CreateGraph( int VertexNum )
{
    Vertex V;
    LGraph Graph;

    Graph = (LGraph)malloc(sizeof(struct GNode));
    Graph->Nv = VertexNum;

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

void InsertEdge(LGraph Graph, Vertex V, Vertex W)
{
    PtrToAdjVNode NewNode;

    NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    NewNode->AdjV = W;
    NewNode->Next = Graph->G[V].FirstEdge;
    Graph->G[V].FirstEdge = NewNode;
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
int L;
Vertex visited[MaxVertexNum] = {0};
void setZero(int VertexNum)
{
    Vertex V;
    for (V = 1; V <= VertexNum; ++V)
        visited[V] = 0;
}

LGraph BuildGraph( int VertexNum )
{
    LGraph Graph;
    Vertex V, W;
    int m, i;

    Graph = CreateGraph(VertexNum);
    for (V = 1; V <= Graph->Nv; ++V) {
        scanf("%d", &m);    // user[V] follows m people
        for (i = 0; i < m; ++i) {
            scanf("%d", &W);
            InsertEdge(Graph, W, V);
        }
    }
    return Graph;
}

int BFS(LGraph Graph, Vertex V, int levelNum)
{
    int count, level;
    Vertex tail, last; Queue Q;
    PtrToAdjVNode Node;
    Q = CreateQueue();
    setZero(Graph->Nv);

    visited[V] = 1; count = 0;
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
        if (level == levelNum) break;
    }

    DestoryQueue(Q);
    return count;
}

void CalForwards(LGraph Graph, int levelNum)
{
    int i, k, count;
    Vertex V;
    scanf("%d", &k);
    for (i = 0; i < k; ++i) {
        scanf("%d", &V);
        count = BFS(Graph, V, levelNum);
        printf("%d\n", count);
    }
}

int main()
{
    int userNum, levelNum;
    LGraph Graph;
    scanf("%d %d", &userNum, &levelNum);
    Graph = BuildGraph(userNum);
    CalForwards(Graph, levelNum);
    DestoryGraph(Graph);

    return 0;
}
