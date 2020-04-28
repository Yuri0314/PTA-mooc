#include <stdio.h>
#include <stdlib.h>

void solve();

int main()
{
    solve();

    return 0;
}

void solve()
{
    int i, n, tmp, num, *arr;

    scanf("%d", &n);
    arr = (int *)malloc(n * sizeof(int));

    for (i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }

    num = 0; i = 0;
    if (arr[0] == 0) i = 1;
    for (; i < n; ++i) {
        if (arr[i] != i) {
            ++num;  // 假设0不在环里，则此处需要将0额外换入该环中，因此多执行一次swap
            while (arr[i] != i) {
                if (arr[i] == 0)    // 说明0在这个环里，无需额外换入环中，抵消外层的+1计数，并且本身无需再加1，因为包含0的环交换次数等于环长度减1
                    --num;
                else
                    ++num;
                tmp = arr[i];
                arr[i] = i;
                i = tmp;
            }
        }
    }

    printf("%d", num);
    free(arr);
}
