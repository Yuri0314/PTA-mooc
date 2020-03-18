#include <stdio.h>
#include <stdlib.h>

typedef struct PolyNode *Polynomial;
struct PolyNode {
    int coef;
    int expon;
    Polynomial link;
};

void Attach( int c, int e, Polynomial *pRear )
{
    Polynomial P;

    P = (Polynomial)malloc(sizeof(struct PolyNode));
    P->coef = c;
    P->expon = e;
    P->link = NULL;
    (*pRear)->link = P;
    *pRear = P;
}

Polynomial ReadPoly()
{
    Polynomial P, Rear, t;
    int c, e, N;

    scanf("%d", &N);
    P = (Polynomial)malloc(sizeof(struct PolyNode));
    P->link = NULL;
    Rear = P;
    while( N-- ) {
        scanf("%d %d", &c, &e);
        Attach(c, e, &Rear);
    }
    t = P; P = P->link; free(t);
    return P;
}

Polynomial Add( Polynomial P1, Polynomial P2 )
{
    Polynomial t1, t2, P, Rear, temp;
    int sum;
    t1 = P1; t2 = P2;
    P = (Polynomial)malloc(sizeof(struct PolyNode)); P->link = NULL;
    Rear = P;
    while (t1 && t2) {
        if (t1->expon == t2->expon) {
            sum = t1->coef + t2->coef;
            if ( sum ) Attach(sum, t1->expon, &Rear);
            t1 = t1->link;
            t2 = t2->link;
        }
        else if (t1->expon > t2->expon) {
            Attach(t1->coef, t1->expon, &Rear);
            t1 = t1->link;
        }
        else {
            Attach(t2->coef, t2->expon, &Rear);
            t2 = t2->link;
        }
    }
    while (t1) {
        Attach(t1->coef, t1->expon, &Rear);
        t1 = t1->link;
    }
    while (t2) {
        Attach(t2->coef, t2->expon, &Rear);
        t2 = t2->link;
    }
    Rear->link = NULL;
    temp = P;
    P = P->link;
    free(temp);
    return P;
}

Polynomial Mult( Polynomial P1, Polynomial P2 )
{
    Polynomial t1, t2, P, Rear, PP;
    PP = NULL;
    t1 = P1; t2 = P2;
    P = (Polynomial)malloc(sizeof(struct PolyNode)); P->link = NULL;
    Rear = P;
    while (t1) {
        t2 = P2; P->link = NULL; Rear = P;
        while (t2) {
            Attach(t1->coef * t2->coef, t1->expon + t2->expon, &Rear);
            t2 = t2->link;
        }
        PP = Add(PP, P->link);
        t1 = t1->link;
    }
    free(P);
    return PP;
}

void PrintPoly( Polynomial P )
{
    int flag = 0;

    if (!P) {printf("0 0\n"); return;}

    while( P ) {
        if (!flag)
            flag = 1;
        else
            printf(" ");
        printf("%d %d", P->coef, P->expon);
        P = P->link;
    }
    printf("\n");
}

int main()
{
    Polynomial P1, P2, PP, PS;

    P1 = ReadPoly();
    P2 = ReadPoly();
    PP = Mult( P1, P2 );
    PrintPoly( PP );
    PS = Add( P1, P2 );
    PrintPoly( PS );

    return 0;
}

