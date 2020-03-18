#include <stdio.h>
#include <stdlib.h>

int MaxSubseqSum4( int A[], int N )
{
    int ThisSum, MaxSum;
    int start, end, MaxStart, MaxEnd;
    int i;
    ThisSum = MaxSum = 0;
    start = MaxStart = 0; MaxEnd = N - 1;
    for ( i = 0; i < N; ++i ) {
        ThisSum += A[i];
        if ( ThisSum > MaxSum || (ThisSum == MaxSum && MaxSum == 0)){
                MaxSum = ThisSum;
                MaxStart = start;
                MaxEnd = i;
            }
        else if ( ThisSum < 0 ) {
            ThisSum = 0;
            start = i + 1;
        }
    }
    printf("%d %d %d", MaxSum, A[MaxStart], A[MaxEnd]);
    return MaxSum;
}

int arr[1000000];

int main()
{
    int K = 0;
    scanf("%d", &K);
    int i = 0;
    for (i = 0; i < K; ++i) {
        scanf("%d", &arr[i]);
    }
    MaxSubseqSum4(arr, K);

    return 0;
}
