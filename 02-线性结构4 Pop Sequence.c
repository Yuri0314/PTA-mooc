#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct SNode *Stack;
struct SNode {
    ElementType *Data;
    int Top;
};

void makeEmpty(Stack stack)
{
    stack->Top = -1;
}

int isEmpty(Stack stack)
{
    return stack->Top == -1 ? 1 : 0;
}

void push(Stack stack, ElementType data, int M)
{
    if (stack->Top == M - 1)
        return;
    else
        stack->Data[++(stack->Top)] = data;
}

ElementType pop(Stack stack)
{
    if (stack->Top == -1)
        return NULL;
    else
        return stack->Data[(stack->Top)--];
}

ElementType getTopElem(Stack stack)
{
    if (stack->Top == -1)
        return NULL;
    return stack->Data[stack->Top];
}

int check(int *seq, int N, Stack stack, int M)
{
    int i, flag;
    ElementType push_item;
    makeEmpty(stack);
    push_item = 1;
    flag = 1;
    for (i = 0; i < N; ++i) {
        while (getTopElem(stack) != seq[i] && push_item <= N) {
            push(stack, push_item++, M);
        }
        if (getTopElem(stack) == seq[i]) {
            pop(stack);
        } else {
            flag = 0;
            break;
        }
    }

    return flag;
}

int main()
{
    int M, N, K, *seq, ans, i, j;
    Stack stack;
    stack = (Stack)malloc(sizeof(struct SNode));
    scanf("%d %d %d", &M, &N, &K);
    stack->Data = (ElementType *)malloc(M * sizeof(ElementType));
    seq = (int *)malloc(N * sizeof(int));
    for (i = 0; i < K; ++i) {
        for (j = 0; j < N; ++j)
            scanf("%d", &seq[j]);
        ans = check(seq, N, stack, M);
        if (ans)
            printf("YES\n");
        else
            printf("NO\n");
    }

    free(stack->Data);
    free(stack);
    free(seq);

    return 0;
}
