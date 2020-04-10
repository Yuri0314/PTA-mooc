#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 1000
#define INF 65535
typedef int Vertex;
typedef int WeightType;

typedef struct ENode *PtrToENode;
struct ENode {
    Vertex V1, V2;
    WeightType weight;
};
typedef PtrToENode Edge;
/*-------------------- 顶点并查集定义 --------------------*/
typedef Vertex SetName;     /* 默认用根结点的下标作为集合名称 */
typedef Vertex SetType[MaxVertexNum]; /* 假设集合元素下标从0开始 */

void InitializeVSet( SetType S, int N )
{ /* 初始化并查集 */
    Vertex X;

    for ( X=0; X<N; X++ ) S[X] = -1;
}

void Union( SetType S, SetName Root1, SetName Root2 )
{ /* 这里默认Root1和Root2是不同集合的根结点 */
    /* 保证小集合并入大集合 */
    if ( S[Root2] < S[Root1] ) { /* 如果集合2比较大 */
        S[Root2] += S[Root1];     /* 集合1并入集合2  */
        S[Root1] = Root2;
    }
    else {                         /* 如果集合1比较大 */
        S[Root1] += S[Root2];     /* 集合2并入集合1  */
        S[Root2] = Root1;
    }
}

SetName Find( SetType S, Vertex X )
{ /* 默认集合元素全部初始化为-1 */
    if ( S[X] < 0 ) /* 找到集合的根 */
        return X;
    else
        return S[X] = Find( S, S[X] ); /* 路径压缩 */
}

int CheckCycle( SetType VSet, Vertex V1, Vertex V2 )
{ /* 检查连接V1和V2的边是否在现有的最小生成树子集中构成回路 */
    Vertex Root1, Root2;

    Root1 = Find( VSet, V1 ); /* 得到V1所属的连通集名称 */
    Root2 = Find( VSet, V2 ); /* 得到V2所属的连通集名称 */

    if( Root1==Root2 ) /* 若V1和V2已经连通，则该边不能要 */
        return 0;
    else { /* 否则该边可以被收集，同时将V1和V2并入同一连通集 */
        Union( VSet, Root1, Root2 );
        return 1;
    }
}
/*-------------------- 并查集定义结束 --------------------*/
int vertexNum, edgeNum;
struct ENode edges[MaxVertexNum * 3];

int comp(const void *a, const void *b)
{
    return (*(Edge)a).weight - (*(Edge)b).weight;
}

void initEdges();
int Kruskal();

int main()
{
    printf("%d", Kruskal());

    return 0;
}

void initEdges()
{
    int i;
    scanf("%d %d", &vertexNum, &edgeNum);
    for (i = 0; i < edgeNum; ++i) {
        scanf("%d %d %d", &(edges[i].V1), &(edges[i].V2), &(edges[i].weight));
        --edges[i].V1;  --edges[i].V2;
    }

    qsort(edges, edgeNum, sizeof(struct ENode), comp);
}

int Kruskal()
{
    SetType VSet;
    struct ENode nextEdge;
    int ECount, i; WeightType totalweight;

    initEdges();
    InitializeVSet(VSet, vertexNum);

    ECount = 0; totalweight = 0; i = 0;
    while (ECount < vertexNum - 1) {
        if (i < edgeNum)
            nextEdge = edges[i++];
        else
            break;
        if (CheckCycle(VSet, nextEdge.V1, nextEdge.V2)) {
            totalweight += nextEdge.weight;
            ++ECount;
        }
    }
    if (ECount < vertexNum - 1)
        totalweight = -1;

    return totalweight;
}
