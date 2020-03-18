#include <stdio.h>
#include <stdlib.h>

#define MaxTree 10
#define ElementType char
#define Tree int
#define Null -1

struct TreeNode
{
    ElementType Element;
    Tree Left;
    Tree Right;
} T1[MaxTree], T2[MaxTree];

Tree BuildTree( struct TreeNode T[] )
{
    char cl, cr;
    Tree Root, N, i;
    int check[MaxTree];
    Root = Null;
    scanf("%d\n", &N);
    if (N) {
        for (i = 0; i < N; ++i) check[i] = 0;
        for (i = 0; i < N; ++i) {
            scanf("%c %c %c\n", &T[i].Element, &cl, &cr);
            if (cl != '-') {
                T[i].Left = cl - '0';
                check[T[i].Left] = 1;
            }
            else T[i].Left = Null;
            if (cr != '-') {
                T[i].Right = cr - '0';
                check[T[i].Right] = 1;
            }
            else T[i].Right = Null;
        }
        for (i = 0; i < N; ++i)
            if (!check[i]) break;
        Root = i;
    }
    return Root;
}

int Isomorphic ( Tree R1, Tree R2 )
{
    if ((R1 == Null) && (R2 == Null))
        return 1;
    if (((R1 == Null) && (R2 != Null)) || ((R1 != Null) && (R2 == Null)))
        return 0;
    if (T1[R1].Element != T2[R2].Element)
        return 0;
    if ((T1[R1].Left == Null) && (T2[R2].Left == Null))
        return Isomorphic( T1[R1].Right, T2[R2].Right );
    if (((T1[R1].Left != Null) && (T2[R2].Left != Null)) &&
        ((T1[T1[R1].Left].Element) == (T2[T2[R2].Left].Element)))
        return ( Isomorphic( T1[R1].Left, T2[R2].Left ) &&
                 Isomorphic( T1[R1].Right, T2[R2].Right));
    else
        return ( Isomorphic( T1[R1].Left, T2[R2].Right) &&
                 Isomorphic( T1[R1].Right, T2[R2].Left));
}

int main()
{
    Tree R1, R2;
    R1 = BuildTree(T1);
    R2 = BuildTree(T2);
    if (Isomorphic(R1, R2)) printf("Yes\n");
    else printf("No\n");

    return 0;
}
