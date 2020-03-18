#include <stdio.h>
#include <stdlib.h>

int MaxSubseqSum4( int A[], int N )
{
    int ThisSum, MaxSum;
    int i;
    ThisSum = MaxSum = 0;
    for ( i = 0; i < N; ++i ) {
        ThisSum += A[i];
        if ( ThisSum > MaxSum )
            MaxSum = ThisSum;
        else if ( ThisSum < 0 )
            ThisSum = 0;
    }
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
    int out = MaxSubseqSum4(arr, K);
    printf("%d\n", out);

    return 0;
}
