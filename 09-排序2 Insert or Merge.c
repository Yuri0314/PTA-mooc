#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;

ElementType *ReadInput(int N);
void solve(ElementType A[], ElementType ansArr[], int N);
void PrintArr(ElementType A[], int N);

void Merge(ElementType A[], ElementType TmpA[], int L, int R, int RightEnd);
void Merge_pass(ElementType A[], ElementType TmpA[], int N, int length);

int Check_Insertion_Sort(ElementType A[], ElementType ansArr[], int N);
void Merge_Sort(ElementType A[], ElementType ansArr[], int N);

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
    else {  // 此时一定是归并排序
        Merge_Sort(A, ansArr, N);
        printf("Merge Sort\n");
        PrintArr(A, N);
    }
}

void Merge(ElementType A[], ElementType TmpA[], int L, int R, int RightEnd)
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

void Merge_pass(ElementType A[], ElementType TmpA[], int N, int length)
{
    int i, j;

    for (i = 0; i <= N - 2 * length; i += 2 * length)
        Merge(A, TmpA, i, i + length, i + 2 * length - 1);
    if (i + length < N)
        Merge(A, TmpA, i, i + length, N - 1);
    else
        for (j = i; j < N; ++j) TmpA[j] = A[j];
}

void Merge_Sort(ElementType A[], ElementType ansArr[], int N)
{
    int i, small_length, length;

    small_length = N;
    length = 1;
    for (i = 0; i < N - 1; ++i) {
        if (ansArr[i] > ansArr[i + 1]) {
            small_length = length < small_length ? length : small_length;
            length = 1;
            continue;
        }
        ++length;
    }

    small_length -= small_length % 2;
    if (small_length < N)
        Merge_pass(ansArr, A, N, small_length);
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
