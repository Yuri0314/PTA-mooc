#include <stdio.h>
#include <stdlib.h>

typedef int Position;
typedef int ElementType;
typedef struct Node *PtrToNode;
struct Node {
    Position Address;
    ElementType Data;
    Position Next;
    PtrToNode   Nextptr;
};
typedef PtrToNode List;

#define MaxSize 100000
struct Node arr[MaxSize];

List ReadList(Position firstAddr, int *length)
{
    int i, list_size;
    Position addr, pos;
    List L, p, temp;
    L = (List)malloc(sizeof(struct Node)); L->Nextptr = NULL;
    p = L;
    memset(&arr, -1, sizeof(struct Node));

    for ( i = 0; i < (*length); ++i ) {
        scanf("%d", &addr);
        arr[addr].Address = addr;
        scanf("%d %d", &arr[addr].Data, &arr[addr].Next);
    }

    list_size = 0;
    pos = firstAddr;
    while (arr[pos].Address != -1) {
        p->Nextptr = &arr[pos];
        p = p->Nextptr;
        ++list_size;
        if (arr[pos].Next == -1)
            break;

        pos = p->Next;
    }

    *length = list_size;
    temp = L; L = L->Nextptr; free(temp);
    return L;
}

List Reverse(List L, int reverse_length)
{
    List p1, p2, p3, rear;
    p1 = L; p2 = L->Nextptr; p3 = p2->Nextptr;

    while (reverse_length--) {
        p2->Nextptr = p1;
        p1 = p2;
        p2 = p3;
        if(p3) p3 = p3->Nextptr;
    }
    rear = L->Nextptr;
    rear->Nextptr = p2;
    L->Nextptr = p1;
    return rear;
}

List ReverseList(List L, int length, int reverse_length)
{
    if (reverse_length == 1)
        return L;

    int t;
    List head, p, temp;
    head = (List)malloc(sizeof(struct Node)); head->Nextptr = L;
    p = head;
    t = length / reverse_length;
    while (t--) {
        p = Reverse(p, reverse_length);
    }
    if (length % reverse_length == 0)
        p->Nextptr = NULL;
    temp = head;
    head = head->Nextptr;
    free(temp);
    return head;
}

void PrintList( List L )
{
    List p;
    p = L;
    while (p) {
        if (!p->Nextptr)
            printf("%.5d %d %d\n", p->Address, p->Data, -1);
        else
            printf("%.5d %d %.5d\n", p->Address, p->Data, p->Nextptr->Address);
        p = p->Nextptr;
    }
}

int main()
{
    Position firstAddr;
    int N, K;
    List L;
    scanf("%d %d %d", &firstAddr, &N, &K);

    L = ReadList(firstAddr, &N);
    L = ReverseList(L, N, K);
    PrintList(L);

    return 0;
}
