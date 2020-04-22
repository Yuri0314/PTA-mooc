#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;

ElementType *ReadInput(int N);
void solve(ElementType A[], ElementType ansArr[], int N);
void PrintArr(ElementType A[], int N);

void PercDown(ElementType A[], int p, int N);

int Check_Insertion_Sort(ElementType A[], ElementType ansArr[], int N);
void Heap_Sort(ElementType A[], int N);

int main()
{
    int N;
    ElementType *arr, *ansArr;
    scanf("%d", &N);
    arr = ReadInput(N);
    ansArr = ReadInput(N);
    solve(arr, ansArr, N);
    free(arr);
    free(ansArr);

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

void solve(ElementType A[], ElementType ansArr[], int N)
{
    if (Check_Insertion_Sort(A, ansArr, N)) {   // 判断是否为插入排序
        printf("Insertion Sort\n");
        PrintArr(ansArr, N);
    }
    else {  // 此时一定是堆排序
        Heap_Sort(ansArr, N);
        printf("Heap Sort\n");
        PrintArr(ansArr, N);
    }
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

int Check_Insertion_Sort(ElementType A[], ElementType ansArr[], int N)
{
    int i, pos, flag;
    ElementType tmp;

    flag = 1;
    for (i = 0; i < N - 1; ++i) {   // 找到第一个非有序位置
        if (ansArr[i] > ansArr[i + 1]) {
            pos = i + 1;
            break;
        }
    }
    for (i = pos; i < N; ++i) {     // 判断非有序部分是否和输入数据对应位置一样
        if (A[i] != ansArr[i]) {
            flag = 0;
            break;
        }
    }
    if (flag) { // 如果是插入排序，进行下一次插入排序操作
        tmp = ansArr[pos];
        for (; pos > 0 && tmp < ansArr[pos - 1]; --pos)
            ansArr[pos] = ansArr[pos - 1];
        ansArr[pos] = tmp;
    }
    return flag;
}

void Heap_Sort(ElementType A[], int N)
{
    ElementType Tmp;
    int i;

    for (i = N - 1; i > 0 && A[i] >= A[0] ; --i) ;
    if (i != 0) {
        Tmp = A[0];
        A[0] = A[i];
        A[i] = Tmp;
        PercDown(A, 0, i);
    }
}
