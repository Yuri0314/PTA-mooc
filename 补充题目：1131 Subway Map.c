#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 65535
/* ——————无向图的邻接表定义开始—————— */
#define MaxVertexNum 10000
typedef int Vertex;

typedef struct ENode *PtrToENode;
struct ENode{
    Vertex V1, V2;
    int lineNo;     // 这条边在几号线上
};
typedef PtrToENode Edge;

typedef struct AdjVNode *PtrToAdjVNode;
struct AdjVNode{
    Vertex AdjV;
    int lineNo;     // 这条边在几号线上
    PtrToAdjVNode Next;
};

typedef struct Vnode{
    PtrToAdjVNode FirstEdge;
} AdjList[MaxVertexNum];

typedef struct GNode *PtrToGNode;
struct GNode{
    AdjList G;
};
typedef PtrToGNode LGraph;

LGraph BuildGraph();
void DestoryGraph( LGraph Graph );
void InsertEdge(LGraph Graph, Edge E);
/* ——————无向图的邻接表定义结束—————— */
/* ——————队列定义开始—————— */
#define ERROR -1
typedef int ElementType;
typedef int Position;
struct QNode {
    ElementType *Data;     /* 存储元素的数组 */
    Position Front, Rear;  /* 队列的头、尾指针 */
    int MaxSize;
};
typedef struct QNode *Queue;

Queue CreateQueue(int MaxSize);
void DestoryQueue( Queue Q );
bool IsFull( Queue Q );
bool Enqueue( Queue Q, ElementType X );
bool IsEmpty( Queue Q );
ElementType Dequeue( Queue Q );
void Clear(Queue Q);
/* ——————队列定义结束—————— */
int dist[MaxVertexNum]; // 路径长度
Vertex path[MaxVertexNum];  // 上一步站点编号
int transferNum[MaxVertexNum];  // 换乘次数
int curLine[MaxVertexNum];  // 当前所在地铁线，随着读取的边而改变

void reset();    // 重设辅助数组的初始值
void printRoute(Vertex source);
void query(LGraph graph, Queue queue);

int main()
{
    LGraph graph;
    Queue queue;
    int queryNum, i;
    graph = BuildGraph();
    queue = CreateQueue(MaxVertexNum + 1);
    scanf("%d", &queryNum);
    for (i = 0; i < queryNum; ++i)
        query(graph, queue);
    DestoryGraph(graph);
    DestoryQueue(queue);

    return 0;
}

/* ——————相关数据结构函数实现开始—————— */
LGraph BuildGraph()
{
    LGraph graph; Edge edge;
    int lineNum, stopNum, i, j;
    Vertex preStop, curStop, V;

    graph = (LGraph)malloc(sizeof(struct GNode));
    for (V = 0; V < MaxVertexNum; ++V) graph->G[V].FirstEdge = NULL;
    edge = (Edge)malloc(sizeof(struct ENode));
    scanf("%d", &lineNum);
    for (i = 1; i <= lineNum; ++i) {
        preStop = -1;
        scanf("%d", &stopNum);
        for (j = 0; j < stopNum; ++j) {
            scanf("%d", &curStop);
            if (preStop != -1) {
                edge->V1 = preStop; edge->V2 = curStop; edge->lineNo = i;
                InsertEdge(graph, edge);
            }
            preStop = curStop;
        }
    }
    free(edge);

    return graph;
}

void DestoryGraph( LGraph Graph )
{
    Vertex V;
    PtrToAdjVNode Node;
    if (!Graph) return;
    for (V = 0; V < MaxVertexNum; ++V) {
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
    NewNode->AdjV = E->V2; NewNode->lineNo = E->lineNo;
    NewNode->Next = Graph->G[E->V1].FirstEdge;
    Graph->G[E->V1].FirstEdge = NewNode;

    NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    NewNode->AdjV = E->V1; NewNode->lineNo = E->lineNo;
    NewNode->Next = Graph->G[E->V2].FirstEdge;
    Graph->G[E->V2].FirstEdge = NewNode;
}

Queue CreateQueue(int MaxSize)
{
    Queue Q = (Queue)malloc(sizeof(struct QNode));
    Q->Data = (int *)malloc(MaxSize * sizeof(int));
    Q->Front = Q->Rear = 0;
    Q->MaxSize = MaxSize;
    return Q;
}

void DestoryQueue( Queue Q )
{
    if (Q) {
        if (Q->Data)
            free(Q->Data);
        free(Q);
    }
}

bool IsFull( Queue Q )
{
    return ((Q->Rear+1)%Q->MaxSize == Q->Front);
}

bool Enqueue( Queue Q, ElementType X )
{
    if ( IsFull(Q) ) return false;
    else {
        Q->Rear = (Q->Rear+1)%Q->MaxSize;
        Q->Data[Q->Rear] = X;
        return true;
    }
}

bool IsEmpty( Queue Q )
{
    return (Q->Front == Q->Rear);
}

ElementType Dequeue( Queue Q )
{
    if ( IsEmpty(Q) ) return ERROR;
    else  {
        Q->Front =(Q->Front+1)%Q->MaxSize;
        return  Q->Data[Q->Front];
    }
}

void Clear(Queue Q)
{
    Q->Front = Q->Rear = 0;
}
/* ——————相关数据结构函数实现结束—————— */

void reset()
{
    Vertex V;
    for (V = 0; V < MaxVertexNum; ++V) {
        dist[V] = 0;
        path[V] = -1;
        transferNum[V] = -1;
        curLine[V] = 0;
    }
}

void printRoute(Vertex source)
{
    Vertex transfer;
    printf("%d\n", dist[source]);
    while (transferNum[source] >= 0) {
        for (transfer = path[source]; transfer != -1 && curLine[source] == curLine[transfer]; transfer = path[transfer]) ;  // 找到换乘站
        printf("Take Line#%d from %04d to %04d.\n", curLine[source], source, transfer);
        source = transfer;
    }
}

void query(LGraph graph, Queue queue)
{
    Vertex source, destination, V;
    int tmpTransfreNum;
    PtrToAdjVNode edge;

    scanf("%d %d", &source, &destination);
    reset();
    Clear(queue);
    Enqueue(queue, destination);
    while(!IsEmpty(queue)) {
        V = Dequeue(queue);
        if (V == source) break;
        for (edge = graph->G[V].FirstEdge; edge; edge = edge->Next) {
            if (dist[edge->AdjV] == 0 && edge->AdjV != destination) {    // 如果没访问过
                dist[edge->AdjV] = dist[V] + 1;
                path[edge->AdjV] = V;
                curLine[edge->AdjV] = edge->lineNo;
                transferNum[edge->AdjV] = (curLine[V] != edge->lineNo) ? transferNum[V] + 1 : transferNum[V];
                Enqueue(queue, edge->AdjV);
            }
            else if (dist[V] + 1 == dist[edge->AdjV]) { // 如果访问过并且是下一层结点
                tmpTransfreNum = (edge->lineNo == curLine[V]) ? transferNum[V] : (transferNum[V] + 1);
                if (tmpTransfreNum < transferNum[edge->AdjV]) {
                    path[edge->AdjV] = V;
                    transferNum[edge->AdjV] = tmpTransfreNum;
                    curLine[edge->AdjV] = edge->lineNo;
                }
            }
        }
    }
    printRoute(source);
}
