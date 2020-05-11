#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 65535
#define ERROR -1
/* ——————无向图的邻接表定义开始—————— */
#define MaxVertexNum 501
typedef int Vertex;
typedef int WeightType;
typedef int DataType;

typedef struct ENode *PtrToENode;
struct ENode{
    Vertex V1, V2;
    WeightType time;
};
typedef PtrToENode Edge;

typedef struct AdjVNode *PtrToAdjVNode;
struct AdjVNode{
    Vertex AdjV;
    WeightType time;
    PtrToAdjVNode Next;
};

typedef struct LastVNode *PtrToLastVNode;
struct LastVNode {
    Vertex LastV;
    PtrToLastVNode Next;
};

typedef struct Vnode{
    DataType BikeNum;
    PtrToAdjVNode FirstEdge;
    PtrToLastVNode FirstLastVertex; // 用于在Dijkstra算法中记录到该结点路径长度相同的所有上一跳结点编号
} AdjList[MaxVertexNum];

typedef struct GNode *PtrToGNode;
struct GNode{
    int Nv;
    int Ne;
    AdjList G;
};
typedef PtrToGNode LGraph;

LGraph CreateGraph(int Nv);
void DestoryGraph( LGraph Graph );
void InsertEdge(LGraph Graph, Edge E);
void InsertLastVertex(LGraph Graph, Vertex V, Vertex Last);
void ClearLastVertex(LGraph Graph, Vertex V);     // 清空传入的结点站当前记录的所有上一跳结点编号
/* ——————无向图的邻接表定义结束—————— */
/* ——————栈定义开始—————— */
typedef Vertex ElementType;
typedef int Position;
struct SNode {
    ElementType *Data; /* 存储元素的数组 */
    Position Top;      /* 栈顶指针 */
    int MaxSize;       /* 堆栈最大容量 */
};
typedef struct SNode *Stack;

Stack CreateStack( int MaxSize );
void DestoryStack(Stack stack);
bool IsFull( Stack S );
bool Push( Stack S, ElementType X );
bool IsEmpty( Stack S );
ElementType Pop( Stack S );
int CopyData(Stack stack, ElementType *data);   // 返回拷贝的data长度
/* ——————栈定义结束—————— */
Vertex problemStation;
DataType perfectNum;
bool collected[MaxVertexNum];
WeightType distTime[MaxVertexNum];

DataType minBackSum, minSendSum;
int steps;
Vertex ansPath[MaxVertexNum];

LGraph buildGraph();
Vertex findMinDist(LGraph graph);
void dijkstra(LGraph graph);
void dfs(LGraph graph, Stack stack, Vertex V, DataType backSum, DataType sendSum);
void printAns();
void solve(LGraph graph);

int main()
{
    LGraph graph;
    graph = buildGraph();
    solve(graph);
    DestoryGraph(graph);

    return 0;
}

/* ——————相关数据结构函数实现开始—————— */
LGraph CreateGraph(int Nv)
{
    Vertex V;
    LGraph Graph;
    Graph = (LGraph)malloc(sizeof(struct GNode));
    Graph->Nv = Nv + 1;
    Graph->Ne = 0;

    for (V = 0; V < Graph->Nv; ++V) {
        Graph->G[V].FirstEdge = NULL;
        Graph->G[V].FirstLastVertex = NULL;
    }

    return Graph;
}

void DestoryGraph( LGraph Graph )
{
    Vertex V;
    PtrToAdjVNode Node;
    PtrToLastVNode LNode;
    if (!Graph) return;
    for (V = 0; V < Graph->Nv; ++V) {
        while (Graph->G[V].FirstEdge) {
            Node = Graph->G[V].FirstEdge;
            Graph->G[V].FirstEdge = Node->Next;
            free(Node);
        }
        while (Graph->G[V].FirstLastVertex) {
            LNode = Graph->G[V].FirstLastVertex;
            Graph->G[V].FirstLastVertex = LNode->Next;
            free(LNode);
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

    NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    NewNode->AdjV = E->V1; NewNode->time = E->time;
    NewNode->Next = Graph->G[E->V2].FirstEdge;
    Graph->G[E->V2].FirstEdge = NewNode;
}

void InsertLastVertex(LGraph Graph, Vertex V, Vertex Last)
{
    PtrToLastVNode NewVertexNode;

    NewVertexNode = (PtrToLastVNode)malloc(sizeof(struct LastVNode));
    NewVertexNode->LastV = Last;
    NewVertexNode->Next = Graph->G[V].FirstLastVertex;
    Graph->G[V].FirstLastVertex = NewVertexNode;
}

void ClearLastVertex(LGraph Graph, Vertex V)
{
    PtrToLastVNode Node;
    while (Graph->G[V].FirstLastVertex) {
        Node = Graph->G[V].FirstLastVertex;
        Graph->G[V].FirstLastVertex = Node->Next;
        free(Node);
    }
    Graph->G[V].FirstLastVertex = NULL;
}

Stack CreateStack( int MaxSize )
{
    Stack S = (Stack)malloc(sizeof(struct SNode));
    S->Data = (ElementType *)malloc(MaxSize * sizeof(ElementType));
    S->Top = -1;
    S->MaxSize = MaxSize;
    return S;
}

void DestoryStack(Stack stack)
{
    if (!stack) return;
    if (stack->Data) free(stack->Data);
    free(stack);
}

bool IsFull( Stack S )
{
    return (S->Top == S->MaxSize-1);
}

bool Push( Stack S, ElementType X )
{
    if ( IsFull(S) ) return false;
    else {
        S->Data[++(S->Top)] = X;
        return true;
    }
}

bool IsEmpty( Stack S )
{
    return (S->Top == -1);
}

ElementType Pop( Stack S )
{
    if ( IsEmpty(S) ) return ERROR;
    else return ( S->Data[(S->Top)--] );
}

int CopyData(Stack stack, ElementType *data)
{
    Position i;
    for (i = 0; i <= stack->Top; ++i)
        data[i] = stack->Data[i];
    return i;
}
/* ——————相关数据结构函数实现结束—————— */

LGraph buildGraph()
{
    LGraph graph;
    Edge edge;
    DataType maxCapacity;
    int Nv, i; Vertex V;

    scanf("%d %d", &maxCapacity, &Nv);
    perfectNum = maxCapacity / 2;
    graph = CreateGraph(Nv);
    scanf("%d %d", &problemStation, &(graph->Ne));
    for (V = 1; V < graph->Nv; ++V)
        scanf("%d", &(graph->G[V].BikeNum));
    edge = (Edge)malloc(sizeof(struct ENode));
    for (i = 0; i < graph->Ne; ++i) {
        scanf("%d %d %d", &(edge->V1), &(edge->V2), &(edge->time));
        InsertEdge(graph, edge);
    }
    free(edge);

    return graph;
}

Vertex findMinDist(LGraph graph)
{
    Vertex minV, V;
    WeightType minDist = INF;

    for (V = 0; V < graph->Nv; ++V) {
        if (!collected[V] && distTime[V] < minDist) {
            minDist = distTime[V];
            minV = V;
        }
    }
    if (minDist < INF) return minV;
    else return ERROR;
}

void dijkstra(LGraph graph)
{
    Vertex V;
    PtrToAdjVNode edge;

    for (V = 0; V < graph->Nv; ++V) {
        collected[V] = false;
        distTime[V] = INF;
    }
    // 从问题站点向PBMC点反向遍历
    distTime[problemStation] = 0;
    while (true) {
        V = findMinDist(graph);
        if (V == ERROR || V == 0) break;
        collected[V] = true;
        for (edge = graph->G[V].FirstEdge; edge; edge = edge->Next) {
            if (collected[edge->AdjV]) continue;
            if (distTime[V] + edge->time < distTime[edge->AdjV]) {
                distTime[edge->AdjV] = distTime[V] + edge->time;
                ClearLastVertex(graph, edge->AdjV);
                InsertLastVertex(graph, edge->AdjV, V);
            }
            else if (distTime[V] + edge->time == distTime[edge->AdjV])
                InsertLastVertex(graph, edge->AdjV, V);
        }
    }
}

void dfs(LGraph graph, Stack stack, Vertex V, DataType backSum, DataType sendSum)
{
    PtrToLastVNode last;
    DataType minus;
    if (V == problemStation) {  // 只有当到达最终问题站时，才能知道这条路径发送的总车数及需要返回的车数
        if (sendSum < minSendSum) {
            minSendSum = sendSum;
            minBackSum = backSum;
            steps = CopyData(stack, ansPath);
        }
        else if (sendSum == minSendSum && backSum < minBackSum) {
            minSendSum = sendSum;
            minBackSum = backSum;
            steps = CopyData(stack, ansPath);
        }
        return;
    }
    for (last = graph->G[V].FirstLastVertex; last; last = last->Next) {
        Push(stack, last->LastV);
        minus = graph->G[last->LastV].BikeNum - perfectNum;
        if (minus < 0 && backSum + minus < 0) { // 如果该站车数少于半满状态，且该路径经过的所有前面站收集的总车数不足以使该站达到perfect
            dfs(graph, stack, last->LastV, 0, sendSum - (backSum + minus));
        }
        else {
            dfs(graph, stack, last->LastV, backSum + minus, sendSum);
        }
        Pop(stack);
    }
}

void printAns()
{
    Vertex V;
    printf("%d 0", minSendSum);
    for (V = 0; V < steps; ++V)
        printf("->%d", ansPath[V]);
    printf(" %d", minBackSum);
}

void solve(LGraph graph)
{
    Stack stack;

    dijkstra(graph);    // 寻找最小路径

    stack = CreateStack(graph->Nv);
    minBackSum = minSendSum = INF;
    dfs(graph, stack, 0, 0, 0); // 寻找最小路径相等时最优的路径
    printAns();
    DestoryStack(stack);
}
