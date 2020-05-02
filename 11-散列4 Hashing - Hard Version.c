#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ——————图的邻接表表示相关定义开始—————— */
#define MaxVertexNum 1000    /* 最大顶点数设为100 */
typedef int Index;         /* 用顶点下标表示顶点,为整型 */
typedef int ElementType;

/* 边的定义 */
typedef struct ENode *PtrToENode;
struct ENode{
    Index V1, V2;      /* 有向边<V1, V2> */
};
typedef PtrToENode Edge;

/* 邻接点的定义 */
typedef struct AdjVNode *PtrToAdjVNode;
struct AdjVNode{
    Index AdjV;        /* 邻接点下标 */
    PtrToAdjVNode Next;    /* 指向下一个邻接点的指针 */
};

/* 顶点表头结点的定义 */
typedef struct Vnode{
    ElementType data;   // 该顶点的数值，-1表示该顶点不存在
    int indegree;   // 该顶点的入度，不存在时为-1
    PtrToAdjVNode FirstEdge;/* 边表头指针 */
} AdjList[MaxVertexNum];    /* AdjList是邻接表类型 */

/* 图结点的定义 */
typedef struct GNode *PtrToGNode;
struct GNode{
    int Nv;     /* 顶点数 */
    AdjList G;  /* 邻接表 */
};
typedef PtrToGNode LGraph; /* 以邻接表方式存储的图类型 */

LGraph CreateGraph( int VertexNum );    // 初始化一个有VertexNum个顶点但没有边的图
void DestoryGraph( LGraph Graph );
void InsertEdge( LGraph Graph, Edge E );
/* ——————图的邻接表表示相关定义结束—————— */
/* ——————堆的相关定义开始—————— */
typedef struct HNode *Heap; /* 堆的类型定义 */
struct HNode {
    Index *Data; /* 存储元素的数组，Data[0]存储元素个数 */
    int Capacity;      /* 堆的最大容量 */
};
typedef Heap MinHeap; /* 最小堆 */

#define ERROR -1 /* 错误标识应根据具体情况定义为堆中不可能出现的元素值 */
MinHeap CreateHeap( int MaxSize );
void DestoryHeap(MinHeap heap);
bool IsFull( MinHeap H );
bool IsEmpty( MinHeap H );
bool Insert( LGraph graph, MinHeap H, Index X );    // 将元素X插入最小堆H
Index DeleteMin( LGraph graph, MinHeap H ); // 从最小堆H中取出键值最小的元素，并删除一个结点
/* ——————堆的相关定义结束—————— */

LGraph buildGraph();
void handleAndPrint(LGraph graph);

int main()
{
    LGraph graph;
    graph = buildGraph();
    handleAndPrint(graph);
    DestoryGraph(graph);

    return 0;
}

LGraph CreateGraph( int VertexNum )
{ /* 初始化一个有VertexNum个顶点但没有边的图 */
    Index V;
    LGraph Graph;

    Graph = (LGraph)malloc( sizeof(struct GNode) ); /* 建立图 */
    Graph->Nv = VertexNum;
    /* 初始化邻接表头指针 */
    /* 注意：这里默认顶点编号从0开始，到(Graph->Nv - 1) */
    for (V=0; V<Graph->Nv; V++) {
        Graph->G[V].data = -1;
        Graph->G[V].indegree = -1;
        Graph->G[V].FirstEdge = NULL;
    }

    return Graph;
}

void DestoryGraph( LGraph Graph )
{
    Index V;
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

void InsertEdge( LGraph Graph, Edge E )
{
    PtrToAdjVNode NewNode;

    /* 插入边 <V1, V2> */
    /* 为V2建立新的邻接点 */
    NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    NewNode->AdjV = E->V2;
    /* 将V2插入V1的表头 */
    NewNode->Next = Graph->G[E->V1].FirstEdge;
    Graph->G[E->V1].FirstEdge = NewNode;
}

MinHeap CreateHeap( int MaxSize )
{
    MinHeap H = (MinHeap)malloc(sizeof(struct HNode));
    H->Data = (ElementType *)malloc((MaxSize+1)*sizeof(ElementType));
    H->Data[0] = 0;
    H->Capacity = MaxSize;

    return H;
}

void DestoryHeap(MinHeap heap)
{
    free(heap->Data);
    free(heap);
}

bool IsFull( MinHeap H )
{
    return (H->Data[0] == H->Capacity);
}

bool Insert( LGraph graph, MinHeap H, Index X )
{
    int i;

    if ( IsFull(H) ) return false;
    i = ++(H->Data[0]); /* i指向插入后堆中的最后一个元素的位置 */
    for ( ; (i / 2 > 0) && graph->G[H->Data[i/2]].data > graph->G[X].data; i/=2 )
        H->Data[i] = H->Data[i/2]; /* 上滤X */
    H->Data[i] = X; /* 将X插入 */
    return true;
}

bool IsEmpty( MinHeap H )
{
    return (H->Data[0] == 0);
}

Index DeleteMin( LGraph graph, MinHeap H )
{
    int Parent, Child;
    Index MinItem, X;

    if ( IsEmpty(H) ) return ERROR;

    MinItem = H->Data[1]; /* 取出根结点存放的最小值 */
    /* 用最小堆中最后一个元素从根结点开始向上过滤下层结点 */
    X = H->Data[H->Data[0]--]; /* 注意当前堆的规模要减小 */
    for( Parent=1; Parent*2<=H->Data[0]; Parent=Child ) {
        Child = Parent * 2;
        if( (Child!=H->Data[0]) && (graph->G[H->Data[Child]].data > graph->G[H->Data[Child+1]].data) )
            Child++;  /* Child指向左右子结点的较小者 */
        if( graph->G[X].data <= graph->G[H->Data[Child]].data ) break; /* 找到了合适位置 */
        else  /* 下滤X */
            H->Data[Parent] = H->Data[Child];
    }
    H->Data[Parent] = X;

    return MinItem;
}

LGraph buildGraph()
{
    int n;
    Index i, j;
    Edge e;
    LGraph graph;

    e = (Edge)malloc(sizeof(struct ENode));
    scanf("%d", &n);
    graph = CreateGraph(n);
    for (i = 0; i < n; ++i) {
        scanf("%d", &(graph->G[i].data));
        if (graph->G[i].data < 0) continue;   // 没有存数值则跳过该次
        j = graph->G[i].data % graph->Nv;
        graph->G[i].indegree = (i - j + graph->Nv) % graph->Nv;
        while (j != i) {
            e->V1 = j; e->V2 = i;
            InsertEdge(graph, e);
            j = (j + 1) % graph->Nv;
        }
    }
    free(e);
    return graph;
}

void handleAndPrint(LGraph graph)
{
    MinHeap heap;
    Index i, pos;
    PtrToAdjVNode tmp;
    bool isFirst;
    heap = CreateHeap(graph->Nv);
    isFirst = true;
    for (i = 0; i < graph->Nv; ++i) {   // 将入度为0的结点放入堆中
        if (graph->G[i].indegree == 0)
            Insert(graph, heap, i);
    }
    while (!IsEmpty(heap)) {    // 进行拓扑排序，使用堆（即优先队列）来代替一般队列
        pos = DeleteMin(graph, heap);
        tmp = graph->G[pos].FirstEdge;
        while (tmp) {
           if (--(graph->G[tmp->AdjV].indegree) == 0)
                Insert(graph, heap, tmp->AdjV);
           tmp = tmp->Next;
        }
        if (isFirst) isFirst = false;
        else printf(" ");
        printf("%d", graph->G[pos].data);
    }
    DestoryHeap(heap);
}
