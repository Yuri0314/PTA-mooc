#include <stdio.h>
#include <stdlib.h>

int comp(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

/* 读入所有输入结点值，并排序 */
void initInputArr(int *arr, int N)
{
    int i;
    for (i = 0; i < N; ++i)
    {
        scanf("%d", &arr[i]);
    }
    qsort(arr, N, sizeof(int), comp);
}

// 不用额外传入数据长度，数据长度存在tree[0]上
void makeCBST(int *tree, int *arr, int pos, int *i)
{
    // 如果树的位置超出了边界或者输入数据已用完，则返回
    if (pos < 1 || pos > tree[0] || (*i) == tree[0]) return;
    else {  // 中序遍历
        makeCBST(tree, arr, pos * 2, i);
        tree[pos] = arr[(*i)++];
        makeCBST(tree, arr, pos * 2 + 1, i);
    }
}

void printCBST(int *tree)
{
    int i;
    for (i = 1; i <= tree[0]; ++i) {
        if (i != 1) printf(" ");
        printf("%d", tree[i]);
    }
}

int main()
{
    int N, pos, i;
    int *arr, *tree;

    scanf("%d", &N);
    arr = (int *)malloc(N * sizeof(int));   // 用来存放输入结点并排序
    tree = (int *)malloc((N + 1) * sizeof(int));    // 存放完全二叉搜索树的所有结点，第一个位置存放结点个数
    tree[0] = N;
    pos = 1, i = 0;

    initInputArr(arr, N);
    makeCBST(tree, arr, pos, &i);
    printCBST(tree);

    free(arr);
    free(tree);

    return 0;
}
