#include <stdio.h>
#include <stdlib.h>

#define Cutoff_50 50
#define Cutoff_100 100
#define Cutoff_200 200
#define Cutoff_300 300
#define Cutoff_400 400

typedef int ElementType;

void Swap (ElementType *a, ElementType *b)
{
    ElementType t = *a; *a = *b; *b = t;
}

int Comp(const void *a, const void *b)
{
    return *(ElementType *)a - *(ElementType *)b;
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

ElementType *ReadInput(int N);
void PrintArr(ElementType A[], int N);

void Insertion_Sort(ElementType A[], int N);
void Quick_Sort(ElementType A[], int N);

void Qsort1(ElementType A[], int Left, int Right);  // 直接选A[0]作为pivot，Cutoff取50
void Qsort2(ElementType A[], int Left, int Right);  // 用随机函数作为pivot，Cutoff取50
void Qsort3(ElementType A[], int Left, int Right);  // 三元选中值作为pivot，Cutoff取50

void Qsort3_Cutoff100(ElementType A[], int Left, int Right);    // 三元选中值作为pivot，Cutoff取100
void Qsort3_Cutoff200(ElementType A[], int Left, int Right);    // 三元选中值作为pivot，Cutoff取200
void Qsort3_Cutoff300(ElementType A[], int Left, int Right);    // 三元选中值作为pivot，Cutoff取300
void Qsort3_Cutoff400(ElementType A[], int Left, int Right);    // 三元选中值作为pivot，Cutoff取400

int main()
{
    int N;
    ElementType *arr;
    scanf("%d", &N);
    arr = ReadInput(N);
    Quick_Sort(arr, N);
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

void Quick_Sort(ElementType A[], int N)
{
    qsort(A, N, sizeof(ElementType), Comp); // 调用系统的快排函数
    // Qsort3(A, 0, N - 1);
}

void Qsort1(ElementType A[], int Left, int Right)
{
    int Pivot, Low, High;

    if (Cutoff_50 <= Right - Left) {
        Pivot = A[Left];
        Low = Left; High = Right + 1;
        while (1) {
            while(A[++Low] < Pivot) ;
            while(A[--High] > Pivot) ;
            if (Low < High) Swap(&A[Low], &A[High]);
            else break;
        }
        Swap(&A[Left], &A[High]);
        Qsort1(A, Left, High - 1);
        Qsort1(A, High + 1, Right);
    }
    else Insertion_Sort(A + Left, Right - Left + 1);
}

void Qsort2(ElementType A[], int Left, int Right)
{
    int Pivot, Low, High, rand_num;

    if (Cutoff_50 <= Right - Left) {
        rand_num = rand() % (Right - Left + 1) + Left;
        Pivot = A[rand_num];
        Swap(&A[Left], &A[rand_num]);
        Low = Left; High = Right + 1;
        while (1) {
            while(A[++Low] < Pivot) ;
            while(A[--High] > Pivot) ;
            if (Low < High) Swap(&A[Low], &A[High]);
            else break;
        }
        Swap(&A[Left], &A[High]);
        Qsort2(A, Left, High - 1);
        Qsort2(A, High + 1, Right);
    }
    else Insertion_Sort(A + Left, Right - Left + 1);
}

void Qsort3(ElementType A[], int Left, int Right)
{
    int Pivot, Low, High;

    if (Cutoff_50 <= Right - Left) {
        Pivot = Median3(A, Left, Right);
        Low = Left; High = Right - 1;
        while (1) {
            while(A[++Low] < Pivot) ;
            while(A[--High] > Pivot) ;
            if (Low < High) Swap(&A[Low], &A[High]);
            else break;
        }
        Swap(&A[Low], &A[Right - 1]);
        Qsort3(A, Left, Low - 1);
        Qsort3(A, Low + 1, Right);
    }
    else Insertion_Sort(A + Left, Right - Left + 1);
}

void Qsort3_Cutoff100(ElementType A[], int Left, int Right)
{
    int Pivot, Low, High;

    if (Cutoff_100 <= Right - Left) {
        Pivot = Median3(A, Left, Right);
        Low = Left; High = Right - 1;
        while (1) {
            while(A[++Low] < Pivot) ;
            while(A[--High] > Pivot) ;
            if (Low < High) Swap(&A[Low], &A[High]);
            else break;
        }
        Swap(&A[Low], &A[Right - 1]);
        Qsort3_Cutoff100(A, Left, Low - 1);
        Qsort3_Cutoff100(A, Low + 1, Right);
    }
    else Insertion_Sort(A + Left, Right - Left + 1);
}

void Qsort3_Cutoff200(ElementType A[], int Left, int Right)
{
    int Pivot, Low, High;

    if (Cutoff_200 <= Right - Left) {
        Pivot = Median3(A, Left, Right);
        Low = Left; High = Right - 1;
        while (1) {
            while(A[++Low] < Pivot) ;
            while(A[--High] > Pivot) ;
            if (Low < High) Swap(&A[Low], &A[High]);
            else break;
        }
        Swap(&A[Low], &A[Right - 1]);
        Qsort3_Cutoff200(A, Left, Low - 1);
        Qsort3_Cutoff200(A, Low + 1, Right);
    }
    else Insertion_Sort(A + Left, Right - Left + 1);
}

void Qsort3_Cutoff300(ElementType A[], int Left, int Right)
{
    int Pivot, Low, High;

    if (Cutoff_300 <= Right - Left) {
        Pivot = Median3(A, Left, Right);
        Low = Left; High = Right - 1;
        while (1) {
            while(A[++Low] < Pivot) ;
            while(A[--High] > Pivot) ;
            if (Low < High) Swap(&A[Low], &A[High]);
            else break;
        }
        Swap(&A[Low], &A[Right - 1]);
        Qsort3_Cutoff300(A, Left, Low - 1);
        Qsort3_Cutoff300(A, Low + 1, Right);
    }
    else Insertion_Sort(A + Left, Right - Left + 1);
}

void Qsort3_Cutoff400(ElementType A[], int Left, int Right)
{
    int Pivot, Low, High;

    if (Cutoff_400 <= Right - Left) {
        Pivot = Median3(A, Left, Right);
        Low = Left; High = Right - 1;
        while (1) {
            while(A[++Low] < Pivot) ;
            while(A[--High] > Pivot) ;
            if (Low < High) Swap(&A[Low], &A[High]);
            else break;
        }
        Swap(&A[Low], &A[Right - 1]);
        Qsort3_Cutoff400(A, Left, Low - 1);
        Qsort3_Cutoff400(A, Low + 1, Right);
    }
    else Insertion_Sort(A + Left, Right - Left + 1);
}

