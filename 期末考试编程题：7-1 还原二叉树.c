#include <stdio.h>
#include <stdlib.h>

#define MAXN 50

int GetHeight(char *a, char *b, int len);   // 获取树的最大高度

int main()
{
    int n;
    char arr1[MAXN + 1], arr2[MAXN + 1];
    scanf("%d", &n);
    scanf("%s\n%s", arr1, arr2);

    printf("%d", GetHeight(arr1, arr2, n));

    return 0;
}

int GetHeight(char *a, char *b, int len)
{
    int i, leftH, rightH;
    if (len == 0) return 0;
    else {
        for (i = 0; i < len; ++i)
            if (b[i] == a[0]) break;
        leftH = GetHeight(a + 1, b, i) + 1;
        rightH = GetHeight(a + 1 + i, b + 1 + i, len - i - 1) + 1;
        return leftH > rightH ? leftH : rightH;
    }
}
