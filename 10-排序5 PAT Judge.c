#include <stdio.h>
#include <stdlib.h>

typedef struct User_Info *Ptr_User_Info;
struct User_Info {
    int total_score;    // 总分，-1表示没有提交过或所有提交均未通过编译
    int score[5];   // 各题得分，-1表示提交但未通过编译，-2表示未提交
    int perfect_num;    // 最佳提交题目数
};
typedef Ptr_User_Info User_Info_Arr;

int user_num, problem_num;
User_Info_Arr info_arr;
int problem_mark[5];    // 存储各题目总分

/* ——————桶排序所需数据结构定义开始—————— */
/* 桶元素结点 */
typedef struct Node *PtrToNode;
struct Node {
    int key;
    PtrToNode next;
};

/* 桶头结点 */
struct HeadNode {
    PtrToNode head, tail;
};
typedef struct HeadNode *Bucket;
/* ——————桶排序所需数据结构定义结束—————— */

void solve();

void ReadData();    // 读取输入并初始化各数据
void LSDRadixSort_Print();    // 次位优先的基数排序，先对最佳答题数进行排序，再对总分进行排序，之后输出结果

int main()
{
    solve();

    return 0;
}

void solve()
{
    ReadData();
    LSDRadixSort_Print();
    free(info_arr);
}

void ReadData()
{
    int submission_num, i, j, user_id, problem_id, partial_score_obtained;

    scanf("%d %d %d", &user_num, &problem_num, &submission_num);
    for (i = 0; i < problem_num; ++i)
        scanf("%d", &problem_mark[i]);
    info_arr = (User_Info_Arr)malloc(user_num * sizeof(struct User_Info));
    // 初始化各用户的各题目得分为-2，总分为-1，即未提交状态
    for (i = 0; i < user_num; ++i) {
        for (j = 0; j < problem_num; ++j)
            info_arr[i].score[j] = -2;
        info_arr[i].total_score = -1;
        info_arr[i].perfect_num = 0;
    }
    for (i = 0; i < submission_num; ++i) {
        scanf("%d %d %d", &user_id, &problem_id, &partial_score_obtained);
        --user_id; --problem_id;
        if (info_arr[user_id].score[problem_id] == -2) { // 如果该题目未提交过，则只要该题目有提交记录，则均初始化为提交但未编译通过状态
            info_arr[user_id].score[problem_id] = -1;
        }
        if (info_arr[user_id].score[problem_id] < partial_score_obtained) {    // 提交编译通过
            if (info_arr[user_id].total_score < 0)  // 若该用户之前没有提交记录或没有任何编译通过提交，则将其置为有编译通过提交，即总分不小于0
                info_arr[user_id].total_score = 0;
            if (info_arr[user_id].score[problem_id] == -1)  // 若该用户该题的提交之前未编译通过，则此处将其置为0分
                info_arr[user_id].score[problem_id] = 0;
            info_arr[user_id].total_score += (partial_score_obtained - info_arr[user_id].score[problem_id]);
            if (partial_score_obtained == problem_mark[problem_id] && info_arr[user_id].score[problem_id] != partial_score_obtained)
                ++(info_arr[user_id].perfect_num);
            info_arr[user_id].score[problem_id] = partial_score_obtained;
        }
    }
}

void LSDRadixSort_Print()
{
    int i, j, total_score, rank, parallel_num;
    Bucket perfectBucket, scoreBucket;
    PtrToNode tmp;

    // 1. 先对最佳答题数建桶排序
    perfectBucket = (Bucket)malloc((problem_num + 1) * sizeof(struct HeadNode));
    for (i = 0; i <= problem_num; ++i)
        perfectBucket[i].head = perfectBucket[i].tail = NULL;
    for (i = 0; i < user_num; ++i) {
        if (info_arr[i].total_score < 0) continue;  // 如果该用户总分小于0，即没有提交或没有编译通过的提交，则不计入排序
        tmp = (PtrToNode)malloc(sizeof(struct Node));
        tmp->key = i;
        tmp->next = NULL;
        if (perfectBucket[info_arr[i].perfect_num].head == NULL)
            perfectBucket[info_arr[i].perfect_num].head = perfectBucket[info_arr[i].perfect_num].tail = tmp;
        else {
            perfectBucket[info_arr[i].perfect_num].tail->next = tmp;
            perfectBucket[info_arr[i].perfect_num].tail = tmp;
        }
    }

    // 2. 再对总得分建桶排序
    total_score = 0;
    for (i = 0; i < problem_num; ++i)   // 计算满分分数
        total_score += problem_mark[i];
    scoreBucket = (Bucket)malloc((total_score + 1) * sizeof(struct HeadNode));
    for (i = 0; i <= total_score; ++i)
        scoreBucket[i].head = scoreBucket[i].tail = NULL;
    for (i = problem_num ; i >= 0; --i) {
        while (perfectBucket[i].head) {
            tmp = perfectBucket[i].head;
            perfectBucket[i].head = perfectBucket[i].head->next;
            tmp->next = NULL;
            if (scoreBucket[info_arr[tmp->key].total_score].head == NULL)
                scoreBucket[info_arr[tmp->key].total_score].head = scoreBucket[info_arr[tmp->key].total_score].tail = tmp;
            else {
                scoreBucket[info_arr[tmp->key].total_score].tail->next = tmp;
                scoreBucket[info_arr[tmp->key].total_score].tail = tmp;
            }
        }
    }

    // 3. 输出结果
    rank = 1;
    for (i = total_score; i >= 0; --i) {
        parallel_num = 0;   // 计算同名次的人数
        while (scoreBucket[i].head) {
            tmp = scoreBucket[i].head;
            scoreBucket[i].head = scoreBucket[i].head->next;
            printf("%d %05d %d", rank, tmp->key + 1, info_arr[tmp->key].total_score);
            for (j = 0; j < problem_num; ++j) {
                if (info_arr[tmp->key].score[j] == -2)
                    printf(" -");
                else if (info_arr[tmp->key].score[j] == -1)
                    printf(" 0");
                else
                    printf(" %d", info_arr[tmp->key].score[j]);
            }
            printf("\n");
            free(tmp);
            ++parallel_num;
        }
        rank += parallel_num;
    }

    free(perfectBucket);
    free(scoreBucket);
}
