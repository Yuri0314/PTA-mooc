#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode *AVLTree;
struct TreeNode {
    int v;
    AVLTree Left, Right;
};

int getHeight( AVLTree T )
{
    if (!T) return 0;
    int lHeight, rHeight;
    lHeight = getHeight(T->Left);
    rHeight = getHeight(T->Right);
    return (lHeight > rHeight ? lHeight : rHeight) + 1;
}

AVLTree NewNode( int V )
{
    AVLTree T = (AVLTree)malloc(sizeof(struct TreeNode));
    T->v = V;
    T->Left = T->Right = NULL;
    return T;
}

AVLTree RR( AVLTree T )
{
    AVLTree tmp;
    tmp = T->Right;
    T->Right = tmp->Left;
    tmp->Left = T;
    return tmp;
}

AVLTree LL( AVLTree T )
{
    AVLTree tmp;
    tmp = T->Left;
    T->Left = tmp->Right;
    tmp->Right = T;
    return tmp;
}

AVLTree RL( AVLTree T )
{
    AVLTree tmp1, tmp2;
    tmp1 = T->Right;
    tmp2 = tmp1->Left;
    tmp1->Left = tmp2->Right;
    T->Right = tmp2->Left;
    tmp2->Left = T;
    tmp2->Right = tmp1;
    return tmp2;
}

AVLTree LR( AVLTree T )
{
    AVLTree tmp1, tmp2;
    tmp1 = T->Left;
    tmp2 = tmp1->Right;
    tmp1->Right = tmp2->Left;
    T->Left = tmp2->Right;
    tmp2->Left = tmp1;
    tmp2->Right = T;
    return tmp2;
}

AVLTree Insert( AVLTree T, int V )
{
    if ( !T ) T = NewNode(V);
    else {
        if ( V > T->v ) {
            T->Right = Insert( T->Right, V );
            if (getHeight(T->Left) - getHeight(T->Right) == -2) {
                if ( V > T->Right->v )  T = RR(T);
                else    T = RL(T);
            }
        }
        else {
            T->Left = Insert( T->Left, V );
            if (getHeight(T->Left) - getHeight(T->Right) == 2) {
                if ( V < T->Left->v )  T = LL(T);
                else    T = LR(T);
            }
        }
    }
    return T;
}

AVLTree MakeTree( int N )
{
    AVLTree T;
    int i, V;

    T = NULL;
    for (i = 0; i < N; ++i) {
        scanf("%d", &V);
        T = Insert(T, V);
    }
    return T;
}

void FreeTree( AVLTree T )
{
    if (T->Left) FreeTree(T->Left);
    if (T->Right) FreeTree(T->Right);
    free(T);
}

int main()
{
    int N;
    AVLTree T;

    scanf("%d", &N);
    if (N) {
        T = MakeTree(N);
        printf("%d", T->v);
        FreeTree(T);
    }

    return 0;
}
