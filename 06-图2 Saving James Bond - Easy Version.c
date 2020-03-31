#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MaxSize 100
#define Diameter 15
#define YES 1
typedef struct coordinate *Coord;
struct coordinate {
    int x;
    int y;
};
int visited[MaxSize] = {0};
Coord center;
Coord *coord_arr;

void readCoord(int num)
{
    int i;
    Coord coord;
    center = (Coord)malloc(sizeof(struct coordinate));
    center->x = 0; center->y = 0;
    coord_arr = (Coord *)malloc(num * sizeof(Coord));
    for (i = 0; i < num; ++i) {
        coord = (Coord)malloc(sizeof(struct coordinate));
        scanf("%d %d", &coord->x, &coord->y);
        coord_arr[i] = coord;
    }
}

void destoryCoord(int num)
{
    int i;
    for (i = 0; i < num; ++i) {
        free(coord_arr[i]);
    }
    free(coord_arr);
    free(center);
}

int Jump(Coord v, Coord w, int dis)
{
    return sqrt((v->x - w->x) * (v->x - w->x) + (v->y - w->y) * (v->y - w->y)) <= dis;
}

int isSafe(int node, int dis)
{
    if ((coord_arr[node]->x + dis) >= 50 || (coord_arr[node]->x - dis) <= -50 ||
        (coord_arr[node]->y + dis) >= 50 || (coord_arr[node]->y - dis) <= -50)
        return 1;
    return 0;
}

int DFS(int node, int num, int dis)
{
    int ans = 0, i;
    visited[node] = 1;
    if (isSafe(node, dis)) ans = 1;
    else {
        for (i = 0; i < num; ++i) {
            if (!visited[i] && Jump(coord_arr[node], coord_arr[i], dis)) {
                ans = DFS(i, num, dis);
                if (ans == YES) break;
            }
        }
    }
    return ans;
}

void Save007(int num, int dis)
{
    int i, ans;
    for (i = 0; i < num; ++i) {
        if (!visited[i] && Jump(center, coord_arr[i], Diameter + dis)) {
            ans = DFS(i, num, dis);
            if (ans == YES) break;
        }
    }
    if (ans == YES) printf("Yes");
    else printf("No");
}

int main()
{
    int num, dis;
    scanf("%d %d", &num, &dis);

    readCoord(num);
    Save007(num, dis);
    destoryCoord(num);

    return 0;
}
