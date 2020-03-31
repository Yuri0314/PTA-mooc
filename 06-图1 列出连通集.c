#include <stdio.h>
#include <stdlib.h>

#define MAXN 10
int graph[MAXN][MAXN] = {0};
int visitedDFS[MAXN] = {0};
int visitedBFS[MAXN] = {0};

/* 队列定义开始 */
#define MaxSize 11
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

void createGraph(int edges)
{
    int i, j, k;
    for (k = 0; k < edges; ++k) {
        scanf("%d %d", &i, &j);
        graph[i][j] = 1;
        graph[j][i] = 1;
    }
}

void DFS(int vertex, int nodes)
{
    int i, j;
    visitedDFS[vertex] = 1;
    printf("%d ", vertex);
    for (i = vertex, j = 0; j < nodes; ++j) {
        if (graph[i][j] == 1 && !visitedDFS[j])
            DFS(j, nodes);
    }
}

void BFS(int vertex, int nodes)
{
    int tmp, i, j;
    Queue Q;
    Q = CreateQueue();
    visitedBFS[vertex] = 1;
    printf("%d ", vertex);
    Enqueue(Q, vertex);
    while(!IsEmpty(Q)) {
        tmp = Dequeue(Q);
        for (i = tmp, j = 0; j < nodes; ++j) {
            if (graph[i][j] == 1 && !visitedBFS[j]) {
                visitedBFS[j] = 1;
                printf("%d ", j);
                Enqueue(Q, j);
            }
        }
    }

    DestoryQueue(Q);
}

void ListComponents (int nodes)
{
    int i;
    for (i = 0; i < nodes; ++i) {
        if (!visitedDFS[i]) {
            printf("{ ");
            DFS(i, nodes);
            printf("}\n");
        }
    }
    for (i = 0; i < nodes; ++i) {
        if (!visitedBFS[i]) {
            printf("{ ");
            BFS(i, nodes);
            printf("}\n");
        }
    }
}

int main()
{
    int N, E;
    scanf("%d %d", &N, &E);

    createGraph(E);
    ListComponents(N);

    return 0;
}
