#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define KEYLENGTH 11
#define MAXD 5
#define MAXTABLESIZE 1000000

/* ——————HashTable定义开始—————— */
typedef char ElementType[KEYLENGTH + 1];
typedef int Index;

typedef struct LNode *PtrToLNode;
struct LNode {
    ElementType Data;
    PtrToLNode Next;
    int Count;
};
typedef PtrToLNode Position;
typedef PtrToLNode List;

typedef struct TblNode *HashTable;
struct TblNode {
    int TableSize;
    List Heads;
};
/* ——————HashTable定义结束—————— */

int NextPrime(int N);
int Hash(int Key, int p);
Position Find(HashTable H, ElementType Key);

HashTable CreateTable(int TableSize);
bool Insert(HashTable H, ElementType Key);
void ScanAndOutput(HashTable H);
void DestoryTable(HashTable H);

int main()
{
    int N, i;
    ElementType Key;
    HashTable H;

    scanf("%d", &N);
    H = CreateTable(N * 2);
    for (i = 0; i < N; ++i) {
        scanf("%s", Key); Insert(H, Key);
        scanf("%s", Key); Insert(H, Key);
    }
    ScanAndOutput(H);
    DestoryTable(H);

    return 0;
}

int NextPrime(int N)
{
    int i, p = (N % 2) ? N + 2 : N + 1;
    while (p <= MAXTABLESIZE) {
        for (i = (int)sqrt(p); i > 2; --i)
            if (!(p % i)) break;
        if (i == 2) break;
        else p += 2;
    }
    return p;
}

int Hash(int Key, int P)
{
    return Key % P;
}

Position Find(HashTable H, ElementType Key)
{
    Position P;
    Index Pos;

    Pos = Hash(atoi(Key + KEYLENGTH - MAXD), H->TableSize);

    P = H->Heads[Pos].Next;
    while(P && strcmp(P->Data, Key))
        P = P->Next;
    return P;
}

HashTable CreateTable(int TableSize)
{
    HashTable H;
    int i;
    H = (HashTable)malloc(sizeof(struct TblNode));
    H->TableSize = NextPrime(TableSize);
    H->Heads = (List)malloc(H->TableSize * sizeof(struct LNode));
    for (i = 0; i < H->TableSize; ++i) {
        H->Heads[i].Data[0] = '\0'; H->Heads[i].Next = NULL;
        H->Heads[i].Count = 0;
    }
    return H;
}

bool Insert(HashTable H, ElementType Key)
{
    Position P, NewCell;
    Index Pos;

    P = Find(H, Key);
    if (!P) {
        NewCell = (Position)malloc(sizeof(struct LNode));
        strcpy(NewCell->Data, Key);
        NewCell->Count = 1;
        Pos = Hash(atoi(Key + KEYLENGTH - MAXD), H->TableSize);
        NewCell->Next = H->Heads[Pos].Next;
        H->Heads[Pos].Next = NewCell;
        return true;
    }
    else {
        ++P->Count;
        return false;
    }
}

void ScanAndOutput(HashTable H)
{
    int i, MaxCnt, PCnt;
    ElementType MinPhone;
    List Ptr;
    MinPhone[0] = '\0';
    MaxCnt = PCnt = 0;
    for (i = 0; i < H->TableSize; ++i) {
        Ptr = H->Heads[i].Next;
        while (Ptr) {
            if (Ptr->Count > MaxCnt) {
                MaxCnt = Ptr->Count;
                strcpy(MinPhone, Ptr->Data);
                PCnt = 1;
            }
            else if (Ptr->Count == MaxCnt) {
                ++PCnt;
                if (strcmp(MinPhone, Ptr->Data) > 0)
                    strcpy(MinPhone, Ptr->Data);
            }
            Ptr = Ptr->Next;
        }
    }
    printf("%s %d", MinPhone, MaxCnt);
    if (PCnt > 1) printf(" %d", PCnt);
    printf("\n");
}

void DestoryTable(HashTable H)
{
    int i;
    Position P, Tmp;

    for (i = 0; i < H->TableSize; ++i) {
        P = H->Heads[i].Next;
        while (P) {
            Tmp = P->Next;
            free(P);
            P = Tmp;
        }
    }
    free(H->Heads);
    free(H);
}
