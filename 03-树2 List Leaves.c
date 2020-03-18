#include <stdio.h>
#include <stdlib.h>

#define MaxTree 10
#define Tree int
#define Null -1

struct TreeNode
{
    Tree Left;
    Tree Right;
} T[MaxTree];

#define MaxSize 11

struct QNode {
    Tree Data[MaxSize];
    int rear;
    int front;
};
typedef struct QNode *Queue;

Queue CreateQueue()
{
    Queue q = (Queue)malloc(sizeof(struct QNode));
    q->front = 0;
    q->rear = 0;
    return q;
}

void DestoryQueue(Queue q)
{
    free(q);
}

void AddQ(Queue PtrQ, Tree item)
{
    if ((PtrQ->rear+1) % MaxSize == PtrQ->front) return;
    PtrQ->rear = (PtrQ->rear + 1) % MaxSize;
    PtrQ->Data[PtrQ->rear] = item;
}

Tree DeleteQ(Queue PtrQ)
{
    if (PtrQ->front == PtrQ->rear) return NULL;
    else {
        PtrQ->front = (PtrQ->front + 1) % MaxSize;
        return PtrQ->Data[PtrQ->front];
    }
}

int IsEmpty(Queue Q)
{
    return Q->front == Q->rear ? 1 : 0;
}

Tree BuildTree()
{
    char cl, cr;
    int N, i;
    Tree Root;
    int check[MaxTree];
    Root = Null;
    scanf("%d", &N);
    if (N) {
        for (i = 0; i < N; ++i) check[i] = 0;
        for (i = 0; i < N; ++i) {
            scanf("\n%c %c", &cl, &cr);
            if (cl != '-') {
                T[i].Left = cl - '0';
                check[T[i].Left] = 1;
            }
            else T[i].Left = Null;
            if (cr != '-') {
                T[i].Right = cr - '0';
                check[T[i].Right] = 1;
            }
            else T[i].Right = Null;
        }
        for (i = 0; i < N; ++i)
            if (!check[i]) break;
        Root = i;
    }
    return Root;
}

void ListLeaves(Tree R)
{
    Queue Q;    Tree tmp;
    int flag;
    if (R == Null) return;
    Q = CreateQueue();
    AddQ(Q, R);
    flag = 1;
    while (!IsEmpty(Q)) {
        tmp = DeleteQ(Q);
        if (T[tmp].Left == Null && T[tmp].Right == Null) {
            if (!flag) printf(" ");
            printf("%d", tmp);
            flag = 0;
        }
        if (T[tmp].Left != Null) AddQ(Q, T[tmp].Left);
        if (T[tmp].Right != Null) AddQ(Q, T[tmp].Right);
    }
}

int main()
{
    Tree R;
    R = BuildTree();
    ListLeaves(R);

    return 0;
}
