#include <stdio.h>
#include <stdlib.h>

#define ElementType int
typedef struct Node NodeType;
struct Node {
    ElementType index;
    int father;    // 是否为父结点
};
NodeType Error = {-1, 0};

#define MaxSize 30
typedef struct SNode *Stack;
struct SNode {
    NodeType Data[MaxSize];
    int Top;
}stack;

void makeEmpty()
{
    stack.Top = -1;
}

void Push(NodeType item)
{
    if (stack.Top == MaxSize - 1) return;
    else stack.Data[++(stack.Top)] = item;
}

NodeType Pop()
{
    if (stack.Top == -1) return Error;
    else return (stack.Data[(stack.Top)--]);
}

NodeType getTopItem()
{
    if (stack.Top == -1) return Error;
    else return (stack.Data[(stack.Top)]);
}

void printPostorder(int N)
{
    int i, flag;
    char preStr[5] = {'\0'}, str[5] = {'\0'};
    NodeType preNode, node, tmp;

    makeEmpty();

    flag = 0;
    scanf("%s %d", preStr, &preNode.index);
    for (i = 1; i < 2 * N; ++i) {
        scanf("%s", str);
        if (str[1] == 'u')
            scanf("%d", &node.index);
        if (preStr[1] == 'u') {   // 只要前一次是push就可直接压栈
            preNode.father = 0;
            Push(preNode);
        }
        else {  // 前一次为pop时
            if (str[1] == 'o') {    // 第二次也为pop时可以安全出栈并输出
                tmp = Pop();
                if (flag) printf(" ");
                printf("%d", tmp.index);
                flag = 1;
                // 如果此时栈顶元素是父节点，则依次输出所有栈顶的父节点
                tmp = getTopItem();
                while (tmp.father) {
                    Pop();
                    if (flag) printf(" ");
                    printf("%d", tmp.index);
                    tmp = getTopItem();
                }
            }
            else {  // 第二次为push时，说明此时的栈顶元素是有右孩子的父节点，需要pop出来修改其father字段再push回栈中
                tmp = Pop();
                tmp.father = 1;
                Push(tmp);
            }
        }
        preStr[1] = str[1]; // 由于每次只根据第二个字符判断是push还是pop，所以只更改第二个字符即可
        preNode = node;
    }
    // 输出栈中所有剩余结点
    tmp = Pop();
    while (tmp.index > 0) {
        if (flag) printf(" ");
        printf("%d", tmp.index);
        tmp = Pop();
    }
}

int main()
{
    int N;
    scanf("%d", &N);

    printPostorder(N);

    return 0;
}
