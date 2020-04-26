#include <stdio.h>
#include <stdlib.h>

#define QuickSort_Cutoff 50 // 快速排序最小排序长度，当待排序列长度小于该数值时，使用插入排序

// 假设元素最多有4位
#define MaxDigit 4
#define Radix 10

typedef int ElementType;

/* ——————桶排序所需数据结构定义开始—————— */
/* 桶元素结点 */
typedef struct Node *PtrToNode;
struct Node {
    int key;
    PtrToNode next;
};

/* 桶头结点 */
struct HeadNode {
    PtrToNode head, tail;
};
typedef struct HeadNode Bucket[Radix];
/* ——————桶排序所需数据结构定义结束—————— */

void Swap (ElementType *a, ElementType *b)
{
    ElementType t = *a; *a = *b; *b = t;
}

ElementType *ReadInput(int N);
void PrintArr(ElementType A[], int N);

void PercDown(ElementType A[], int p, int N);   // 堆排序使用函数
void Merge_Recursive(ElementType A[], ElementType TmpA[], int L, int R, int RightEnd);  // 递归的归并排序使用函数
void Merge_NotRecursive(ElementType A[], ElementType TmpA[], int L, int R, int RightEnd);   // 非递归的归并排序使用函数
void Msort(ElementType A[], ElementType TmpA[], int L, int RightEnd);   // 递归的归并排序使用函数
void Merge_pass(ElementType A[], ElementType TmpA[], int N, int length);    // 非递归的归并排序使用函数
ElementType Median3( ElementType A[], int Left, int Right );    // 快速排序中的选主元函数，此处使用三数取中法
void Qsort(ElementType A[], int Left, int Right);   // 快速排序的递归函数
int GetDigit(int X, int D);     // 基数排序中用于获取对应位的数字的函数
void MSD(ElementType A[], int L, int R, int D);     // 主位优先的基数排序的核心递归函数

// 各排序算法实现接口及对应的平均时间复杂度说明
void Selection_Sort(ElementType A[], int N);    // 简单选择排序，O(n^2)
void Bubble_Sort(ElementType A[], int N);   // 冒泡排序，O(n^2)，交换变量较为耗时，测试点4、6和8超时过不了
void Insertion_Sort(ElementType A[], int N);    // 插入排序，O(n^2)
void Shell_Sort(ElementType A[], int N);    // 希尔排序，O(n^(1.3—2))
void Heap_Sort(ElementType A[], int N);     // 堆排序，O(n*logn)
void Merge_Sort_Recursive(ElementType A[], int N);  // 递归的归并排序，O(n*logn)
void Merge_Sort_NotRecursive(ElementType A[], int N);  // 非递归的归并排序，O(n*logn)
void Quick_Sort(ElementType A[], int N);    // 快速排序，O(n*logn)
// 基数排序仅能处理正数值区间的数据，因此只看测试点8的结果即可
void LSD_Radix_Sort(ElementType A[], int N);    // 次位优先的基数排序，O(P*(N+B))，P为趟数，B为桶个数
void MSD_Radix_Sort(ElementType A[], int N);    // 主位优先的基数排序

int main()
{
    int N;
    ElementType *arr;
    scanf("%d", &N);
    arr = ReadInput(N);
    LSD_Radix_Sort(arr, N);
    PrintArr(arr, N);
    free(arr);

    return 0;
}

ElementType *ReadInput(int N)
{
    ElementType *arr;
    int i;
    arr = (ElementType *)malloc(sizeof(ElementType) * N);
    for (i = 0 ; i < N; ++i)
        scanf("%d", &arr[i]);
    return arr;
}

void PrintArr(ElementType A[], int N)
{
    int i;
    for ( i = 0; i < N - 1; ++i)
        printf("%d ", A[i]);
    printf("%d", A[i]);
}

void PercDown(ElementType A[], int p, int N)
{
    int Parent, Child;
    ElementType X;

    X = A[p];
    for (Parent = p; (Parent * 2 + 1) < N; Parent = Child) {
        Child = Parent * 2 + 1;
        if ((Child != N - 1) && (A[Child] < A[Child + 1]))
            ++Child;
        if (X >= A[Child]) break;
        else
            A[Parent] = A[Child];
    }
    A[Parent] = X;
}

void Merge_Recursive(ElementType A[], ElementType TmpA[], int L, int R, int RightEnd)
{
    int LeftEnd, NumElements, Tmp;
    int i;

    LeftEnd = R - 1;
    Tmp = L;
    NumElements = RightEnd - L + 1;

    while (L <= LeftEnd && R <= RightEnd) {
        if (A[L] < A[R])
            TmpA[Tmp++] = A[L++];
        else
            TmpA[Tmp++] = A[R++];
    }
    while (L <= LeftEnd) TmpA[Tmp++] = A[L++];
    while (R <= RightEnd) TmpA[Tmp++] = A[R++];

    for (i = 0; i < NumElements; ++i, --RightEnd)
        A[RightEnd] = TmpA[RightEnd];
}

void Merge_NotRecursive(ElementType A[], ElementType TmpA[], int L, int R, int RightEnd)
{
    int LeftEnd, Tmp;

    LeftEnd = R - 1;
    Tmp = L;

    while (L <= LeftEnd && R <= RightEnd) {
        if (A[L] < A[R])
            TmpA[Tmp++] = A[L++];
        else
            TmpA[Tmp++] = A[R++];
    }
    while (L <= LeftEnd) TmpA[Tmp++] = A[L++];
    while (R <= RightEnd) TmpA[Tmp++] = A[R++];
}

void Msort(ElementType A[], ElementType TmpA[], int L, int RightEnd)
{
    int Center;

    if (L < RightEnd) {
        Center = (L + RightEnd) / 2;
        Msort(A, TmpA, L, Center);
        Msort(A, TmpA, Center + 1, RightEnd);
        Merge_Recursive(A, TmpA, L, Center + 1, RightEnd);
    }
}

void Merge_pass(ElementType A[], ElementType TmpA[], int N, int length)
{
    int i, j;

    for (i = 0; i <= N - 2 * length; i += 2 * length)
        Merge_NotRecursive(A, TmpA, i, i + length, i + 2 * length - 1);
    if (i + length < N)
        Merge_NotRecursive(A, TmpA, i, i + length, N - 1);
    else
        for (j = i; j < N; ++j) TmpA[j] = A[j];
}

ElementType Median3( ElementType A[], int Left, int Right )
{
    int Center = (Left+Right) / 2;
    if ( A[Left] > A[Center] )
        Swap( &A[Left], &A[Center] );
    if ( A[Left] > A[Right] )
        Swap( &A[Left], &A[Right] );
    if ( A[Center] > A[Right] )
        Swap( &A[Center], &A[Right] );
    Swap( &A[Center], &A[Right-1] );
    return  A[Right-1];
}

void Qsort(ElementType A[], int Left, int Right)
{
    int Pivot, Low, High;

    if (QuickSort_Cutoff <= Right - Left) {
        Pivot = Median3(A, Left, Right);
        Low = Left; High = Right - 1;
        while (1) {
            while(A[++Low] < Pivot) ;
            while(A[--High] > Pivot) ;
            if (Low < High) Swap(&A[Low], &A[High]);
            else break;
        }
        Swap(&A[Low], &A[Right - 1]);
        Qsort(A, Left, Low - 1);
        Qsort(A, Low + 1, Right);
    }
    else Insertion_Sort(A + Left, Right - Left + 1);
}

int GetDigit ( int X, int D )
{ /* 默认次位D=1, 主位D<=MaxDigit */
    int d, i;

    for (i = 1; i <= D; ++i) {
        d = X % Radix;
        X /= Radix;
    }
    return d;
}

void MSD( ElementType A[], int L, int R, int D )
{ /* 核心递归函数: 对A[L]...A[R]的第D位数进行排序 */
     int Di, i, j;
     Bucket B;
     PtrToNode tmp, p, List = NULL;
     if (D == 0) return; /* 递归终止条件 */

     for (i = 0; i < Radix; ++i) /* 初始化每个桶为空链表 */
         B[i].head = B[i].tail = NULL;
     for (i = L; i <= R; ++i) { /* 将原始序列逆序存入初始链表List */
         tmp = (PtrToNode)malloc(sizeof(struct Node));
         tmp->key = A[i];
         tmp->next = List;
         List = tmp;
     }
     /* 下面是分配的过程 */
     p = List;
     while (p) {
         Di = GetDigit(p->key, D); /* 获得当前元素的当前位数字 */
         /* 从List中摘除 */
         tmp = p; p = p->next;
         /* 插入B[Di]号桶 */
         if (B[Di].head == NULL) B[Di].tail = tmp;
         tmp->next = B[Di].head;
         B[Di].head = tmp;
     }
     /* 下面是收集的过程 */
     i = j = L; /* i, j记录当前要处理的A[]的左右端下标 */
     for (Di = 0; Di < Radix; ++Di) { /* 对于每个桶 */
         if (B[Di].head) { /* 将非空的桶整桶倒入A[], 递归排序 */
             p = B[Di].head;
             while (p) {
                 tmp = p;
                 p = p->next;
                 A[j++] = tmp->key;
                 free(tmp);
             }
             /* 递归对该桶数据排序, 位数减1 */
             MSD(A, i, j-1, D-1);
             i = j; /* 为下一个桶对应的A[]左端 */
         }
     }
}

void Selection_Sort(ElementType A[], int N)
{
    int i, j, min_index;

    for (i = 0; i < N; ++i) {
        min_index = i;
        for (j = i + 1; j < N; ++j)
            if (A[j] < A[min_index])
                min_index = j;
        Swap(&A[i], &A[min_index]);
    }
}

void Bubble_Sort(ElementType A[], int N)
{
    int P, i, flag;
    for (P = N - 1; P > 0; --P) {
        flag = 0;
        for (i = 0; i < P; ++i) {
            if (A[i] > A[i + 1]) {
                Swap(&A[i], &A[i + 1]);
                flag = 1;
            }
        }
        if (!flag) break;
    }
}

void Insertion_Sort(ElementType A[], int N)
{
    int i, P;
    ElementType Tmp;

    for (P = 1; P < N; ++P) {
        Tmp = A[P];
        for (i = P; i > 0 && A[i - 1] > Tmp; --i)
            A[i] = A[i - 1];
        A[i] = Tmp;
    }
}

void Shell_Sort(ElementType A[], int N)
{
    int Si, D, i, P;
    ElementType Tmp;
    int Sedgewick[] = {929, 505, 209, 109, 41, 19, 5, 1, 0};

    for (Si = 0; Sedgewick[Si] >= N; ++Si);

    for (D = Sedgewick[Si]; D > 0; D = Sedgewick[++Si]) {
        for (P = D; P < N; ++P) {
            Tmp = A[P];
            for (i = P; i >= D && A[i - D] > Tmp; i -= D)
                A[i] = A[i - D];
            A[i] = Tmp;
        }
    }
}

void Heap_Sort(ElementType A[], int N)
{
    int i;

    for (i = N / 2 - 1; i >= 0; --i)
        PercDown(A, i, N);

    for (i = N - 1; i > 0; --i) {
        Swap(&A[0], &A[i]);
        PercDown(A, 0, i);
    }
}

void Merge_Sort_Recursive(ElementType A[], int N)
{
    ElementType *TmpA;
    TmpA = (ElementType *)malloc(N * sizeof(ElementType));

    if (TmpA != NULL) {
        Msort(A, TmpA, 0, N - 1);
        free(TmpA);
    }
}

void Merge_Sort_NotRecursive(ElementType A[], int N)
{
    int length;
    ElementType *TmpA;

    length = 1;
    TmpA = (ElementType *)malloc(N * sizeof(ElementType));
    if (TmpA != NULL) {
        while (length < N) {
            Merge_pass(A, TmpA, N, length);
            length *= 2;
            Merge_pass(TmpA, A, N, length);
            length *= 2;
        }
        free(TmpA);
    }
}

void Quick_Sort(ElementType A[], int N)
{
    Qsort(A, 0, N - 1);
}

void LSD_Radix_Sort(ElementType A[], int N)
{
     int D, Di, i;
     Bucket B;
     PtrToNode tmp, p, List = NULL;

     for (i = 0; i < Radix; ++i) /* 初始化每个桶为空链表 */
         B[i].head = B[i].tail = NULL;
     for (i = 0; i < N; ++i) { /* 将原始序列逆序存入初始链表List */
         tmp = (PtrToNode)malloc(sizeof(struct Node));
         tmp->key = A[i];
         tmp->next = List;
         List = tmp;
     }
     /* 下面开始排序 */
     for (D = 1; D <= MaxDigit; ++D) { /* 对数据的每一位循环处理 */
         /* 下面是分配的过程 */
         p = List;
         while (p) {
             Di = GetDigit(p->key, D); /* 获得当前元素的当前位数字 */
             /* 从List中摘除 */
             tmp = p; p = p->next;
             /* 插入B[Di]号桶尾 */
             tmp->next = NULL;
             if (B[Di].head == NULL)
                 B[Di].head = B[Di].tail = tmp;
             else {
                 B[Di].tail->next = tmp;
                 B[Di].tail = tmp;
             }
         }
         /* 下面是收集的过程 */
         List = NULL;
         for (Di = Radix-1; Di >= 0; --Di) { /* 将每个桶的元素顺序收集入List */
             if (B[Di].head) { /* 如果桶不为空 */
                 /* 整桶插入List表头 */
                 B[Di].tail->next = List;
                 List = B[Di].head;
                 B[Di].head = B[Di].tail = NULL; /* 清空桶 */
             }
         }
     }
     /* 将List倒入A[]并释放空间 */
     for (i = 0; i < N; ++i) {
        tmp = List;
        List = List->next;
        A[i] = tmp->key;
        free(tmp);
     }
}

void MSD_Radix_Sort(ElementType A[], int N)
{
    MSD(A, 0, N-1, MaxDigit);
}
