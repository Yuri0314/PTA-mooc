#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINH -1
#define MAXCODESIZE 63

typedef struct TreeNode *HuffmanTree;
struct TreeNode {
    int weight;
    HuffmanTree Left, Right;
};

typedef struct TreeNode *ElementType;
typedef struct HeapStruct *MinHeap;
struct HeapStruct {
    ElementType *Elements;
    int Size;
    int Capacity;
};

/* 堆的相关操作：开始 */
MinHeap CreateHeap( int MaxSize )
{
    MinHeap H = (MinHeap)malloc(sizeof(struct HeapStruct));
    H->Elements = malloc((MaxSize + 1) * sizeof(ElementType));
    H->Size = 0;
    H->Capacity = MaxSize;
    H->Elements[0] = (ElementType)malloc(sizeof(struct TreeNode));
    H->Elements[0]->weight = MINH;

    return H;
}

void DestoryHeap(MinHeap H)
{
    int i;
    if (!H) return;
    if (H->Elements) {
        for (i = 0; i < H->Size; ++i)
            free(H->Elements[i]);
        free(H->Elements);
    }
    free(H);
}

int IsFull(MinHeap H)
{
    return (H->Size == H->Capacity);
}

int IsEmpty(MinHeap H)
{
    return (H->Size == 0);
}

void Insert(MinHeap H, ElementType X)
{
    int i;
    if (IsFull(H)) return;
    i = ++H->Size;
    for (; H->Elements[i / 2]->weight > X->weight; i /= 2)
        H->Elements[i] = H->Elements[i / 2];
    H->Elements[i] = X;
}

ElementType DeleteMin(MinHeap H)
{
    int Parent, Child;
    ElementType MinItem, X;
    if (IsEmpty(H)) return 0;

    MinItem = H->Elements[1];
    X = H->Elements[H->Size--];
    for (Parent = 1; Parent * 2 <= H->Size; Parent = Child) {
        Child = Parent * 2;
        if ((Child != H->Size) && (H->Elements[Child]->weight > H->Elements[Child + 1]->weight))
            Child++;
        if (X->weight <= H->Elements[Child]->weight) break;
        else
            H->Elements[Parent] = H->Elements[Child];
    }
    H->Elements[Parent] = X;
    return MinItem;
}

MinHeap BuildMinHeap(int *freq_arr,int n)
{
    int i;
    ElementType data;
    MinHeap H;
    H = CreateHeap(n);

    for (i = 0; i < H->Capacity; ++i) {
        data = (ElementType)malloc(sizeof(struct TreeNode));
        data->weight = freq_arr[i];
        data->Left = 0; data->Right = 0;
        Insert(H, data);
    }
    return H;
}
/* 堆的相关操作：结束 */

void DestoryCharFreq(int *freq_arr)
{
    if (!freq_arr) return;
    free(freq_arr);
}

void DestoryHuffmanTree(HuffmanTree HT)
{
    if (!HT) return;
    DestoryHuffmanTree(HT->Left);
    DestoryHuffmanTree(HT->Right);
    free(HT);
}

int *readPair(int n)
{
    int i; char c;
    int *freq_arr;
    freq_arr = malloc(n * sizeof(int));
    for (i = 0; i < n; ++i) {
        scanf(" %c %d", &c, &freq_arr[i]);
    }
    return freq_arr;
}

int HuffmanTreeWPL(int *freq_arr, int n)
{
    int i, wpl = 0;
    MinHeap H;
    HuffmanTree T;
    H = BuildMinHeap(freq_arr, n);  // 根据读入数据建立小顶堆
    for (i = 1; i < H->Capacity; ++i) {
        T = (HuffmanTree)malloc(sizeof(struct TreeNode));
        T->Left = DeleteMin(H);
        T->Right = DeleteMin(H);
        T->weight = T->Left->weight + T->Right->weight;
        Insert(H, T);
        wpl += T->weight;
    }
    T = DeleteMin(H);
    DestoryHeap(H);
    DestoryHuffmanTree(T);    // 销毁Huffman树所占空间
    return wpl;
}

void init_codeArr(char *code)   // 初始化code数组每个元素为\0
{
    int i;
    for (i = 0; i < MAXCODESIZE && code[i] != '\0'; ++i)
        code[i] = '\0';
}

HuffmanTree createTreeNode()
{
    HuffmanTree HT;
    HT = (HuffmanTree)malloc(sizeof(struct TreeNode));
    HT->weight = 1; HT->Left = 0; HT->Right = 0;
    return HT;
}

// 这里使用struct TreeNode结构体，其中的weight用来表示是否为本次code新添加结点，若是则为1，否则为0
HuffmanTree RecoverHFTreeByCode(HuffmanTree HT, char *code, int *flag, int *counter)
{
    int i;  HuffmanTree node;
    if (!HT) {
        HT = createTreeNode();
        ++(*counter);
    }
    for (i = 0, node = HT; code[i] != '\0';  ++i) {
        // 第一种情况：该结点不是新添加结点，并且没有左右孩子，即之前字符对应的子节点
        // 说明之前某字符编码是该编码的前缀码
        if (node->weight == 0 && !node->Left && !node->Right) {
            (*flag) = 0;
            break;
        }
        node->weight = 0;
        if (code[i] == '0') {   // 读到0向左孩子走一位
            if (!node->Left) {
                node->Left = createTreeNode();
                ++(*counter);
            }
            node = node->Left;
        } else {
            if (!node->Right) {
                node->Right = createTreeNode();
                ++(*counter);
            }
            node = node->Right;
        }
    }
    // 第二种情况：读完所有code后，该位置有孩子结点，说明该编码是之前某字符编码的前缀码
    if (node->Left || node->Right)
        (*flag) = 0;
    return HT;
}

void check_code(int *freq_arr, int n, int wpl)
{
    int i, sum_wpl, flag, counter; char c;
    HuffmanTree HT;
    char code[MAXCODESIZE] = "\0";
    sum_wpl = 0; flag = 1; counter = 0; HT = 0;
    for (i = 0; i < n; ++i) {
        init_codeArr(code);
        scanf("\n%c %s", &c, code);
        if (flag) { // 判断该次提交是否已经不正确了，如果还正确则继续处理
            sum_wpl += strlen(code) * freq_arr[i];
            HT = RecoverHFTreeByCode(HT, code, &flag, &counter);
        }
    }
    // 这里有三个判断条件
    // 1. 提交的总wpl与预设的完全相同
    // 2. 没有前缀码情况出现，此处用flag标识
    // 3. 没有度为1的结点，此处的counter表示生成的Huffman树结点数，如果正确应该等于2*n-1，n为叶节点个数，即所有字符数
    if (sum_wpl == wpl && flag && counter == 2 * n - 1)
        printf("Yes\n");
    else
        printf("No\n");
    DestoryHuffmanTree(HT);
}

int main()
{
    int n, m, i, wpl;
    int *freq_arr;
    scanf("%d\n", &n);

    freq_arr = readPair(n);     // 读入所有字符及其对应频率
    wpl = HuffmanTreeWPL(freq_arr, n);    // 根据读入字符及频率使用小顶堆建立Huffman树求得WPL值

    scanf("%d", &m);
    for (i = 0; i < m; ++i) {
        check_code(freq_arr, n, wpl);
    }

    DestoryCharFreq(freq_arr);

    return 0;
}
