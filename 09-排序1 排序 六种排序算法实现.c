#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;

void Swap (ElementType *a, ElementType *b)
{
    ElementType t = *a; *a = *b; *b = t;
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

ElementType *ReadInput(int N);
void PrintArr(ElementType A[], int N);

void Bubble_Sort(ElementType A[], int N);   // Ã°ÅÝÅÅÐò£¬O(n^2)£¬½»»»±äÁ¿½ÏÎªºÄÊ±£¬³¬Ê±¹ý²»ÁË
void Insertion_Sort(ElementType A[], int N);    // ²åÈëÅÅÐò£¬O(n^2)
void Shell_Sort(ElementType A[], int N);    //Ï£¶ûÅÅÐò£¬O(n^(1.3¡ª2))
void Heap_Sort(ElementType A[], int N);     // ¶ÑÅÅÐò£¬O(n*logn)
void Merge_Sort_Recursive(ElementType A[], int N);  // µÝ¹éµÄ¹é²¢ÅÅÐò£¬O(n*logn)
void Merge_Sort_NotRecursive(ElementType A[], int N);  // ·ÇµÝ¹éµÄ¹é²¢ÅÅÐò£¬O(n*logn)

int main()
{
    int N;
    ElementType *arr;
    scanf("%d", &N);
    arr = ReadInput(N);
    Merge_Sort_NotRecursive(arr, N);
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
