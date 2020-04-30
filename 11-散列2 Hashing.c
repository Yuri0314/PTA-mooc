#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXTABLESIZE 100000 /* 允许开辟的最大散列表长度 */
typedef int ElementType;    /* 关键词类型用整型 */
typedef int Index;          /* 散列地址类型 */
typedef Index Position;     /* 数据所在位置与散列地址是同一类型 */
/* 散列单元状态类型，分别对应：有合法元素、空单元、有已删除元素 */
typedef enum { Legitimate, Empty } EntryType;

typedef struct HashEntry Cell; /* 散列表单元类型 */
struct HashEntry{
    ElementType Data; /* 存放元素 */
    EntryType Info;   /* 单元状态 */
};

typedef struct TblNode *HashTable; /* 散列表类型 */
struct TblNode {   /* 散列表结点定义 */
    int TableSize; /* 表的最大长度 */
    Cell *Cells;   /* 存放散列单元数据的数组 */
};

int NextPrime(int N);   // 返回大于N且不超过MAXTABLESIZE的最小素数
HashTable CreateTable( int TableSize );
void DestoryTable(HashTable H);
int Hash(int Key, int p);

Position Find( HashTable H, ElementType Key );
Position Insert( HashTable H, ElementType Key );

int main()
{
    int TableSize, N, i, tmp;
    Position pos;
    HashTable H;

    scanf("%d %d", &TableSize, &N);
    H = CreateTable(TableSize);
    for (i = 0; i < N; ++i) {
        scanf("%d", &tmp);
        pos = Insert(H, tmp);
        if (i != 0) printf(" ");
        if (pos == -1)
            printf("-");
        else
            printf("%d", pos);
    }
    DestoryTable(H);

    return 0;
}

int NextPrime( int N )
{
    int i, p;

    if (N < 2) return 2;
    else p = (N%2)? N+2 : N+1; /*从大于N的下一个奇数开始 */
    while( p <= MAXTABLESIZE ) {
        for( i=(int)sqrt(p); i>1; i-- )
            if ( !(p%i) ) break; /* p不是素数 */
        if ( i==1 ) break; /* for正常结束，说明p是素数 */
        else  p += 2; /* 否则试探下一个奇数 */
    }
    return p;
}

HashTable CreateTable( int TableSize )
{
    HashTable H;
    int i;

    H = (HashTable)malloc(sizeof(struct TblNode));
    /* 保证散列表最大长度是素数 */
    H->TableSize = NextPrime(TableSize);
    /* 声明单元数组 */
    H->Cells = (Cell *)malloc(H->TableSize*sizeof(Cell));
    /* 初始化单元状态为“空单元” */
    for( i=0; i<H->TableSize; i++ )
        H->Cells[i].Info = Empty;

    return H;
}

void DestoryTable(HashTable H)
{
    if (!H)
        free(H->Cells);
    free(H);
}

int Hash(int Key, int P)
{
    return Key % P;
}

Position Find( HashTable H, ElementType Key )
{
    Position CurrentPos, NewPos;
    int CNum = 0; /* 记录冲突次数 */

    NewPos = CurrentPos = Hash( Key, H->TableSize ); /* 初始散列位置 */
    /* 当该位置的单元非空，并且不是要找的元素时，发生冲突 */
    while( H->Cells[NewPos].Info!=Empty && H->Cells[NewPos].Data!=Key ) {
                                           /* 字符串类型的关键词需要 strcmp 函数!! */
        NewPos = CurrentPos + (CNum+1)*(CNum+1);
        if ( NewPos >= H->TableSize ) {
            NewPos = NewPos % H->TableSize; /* 调整为合法地址 */
            if (NewPos == CurrentPos)
                break;
        }
        ++CNum;
    }
    return NewPos; /* 此时NewPos或者是Key的位置，或者是一个空单元的位置（表示找不到）*/
}

Position Insert( HashTable H, ElementType Key )
{
    Position Pos = Find( H, Key ); /* 先检查Key是否已经存在 */

    if( H->Cells[Pos].Info != Legitimate ) { /* 如果这个单元没有被占，说明Key可以插入在此 */
        H->Cells[Pos].Info = Legitimate;
        H->Cells[Pos].Data = Key;
        return Pos;
    }
    else
        return -1;
}
