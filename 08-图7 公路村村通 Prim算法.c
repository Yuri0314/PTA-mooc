#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 1000
#define INF 65535
int vertexNum;
int graph[MaxVertexNum][MaxVertexNum];
int dist[MaxVertexNum];

void buildGraph();
void initDist();
int findMinDist();
int prim();

int main()
{
    buildGraph();
    printf("%d", prim());

    return 0;
}

void buildGraph()
{
    int edgeNum, i, end1, end2, weight;

    scanf("%d %d", &vertexNum, &edgeNum);
    for (i = 0; i < edgeNum; ++i) {
        scanf("%d %d %d", &end1, &end2, &weight);
        graph[end1 - 1][end2 - 1] = weight;
        graph[end2 - 1][end1 - 1] = weight;
    }
}

void initDist()
{
    int i;
    for (i = 0; i < vertexNum; ++i) {
        dist[i] = INF;
    }
}

int findMinDist()
{
    int minV, i, minDist = INF;

    for (i = 0;i < vertexNum; ++i) {
        if (dist[i] != 0 && dist[i] < minDist) {
            minDist = dist[i];
            minV = i;
        }
    }
    if (minDist < INF)
        return minV;
    else return -1;
}

int prim()
{
    int i, totalWeight, vCount, v;

    initDist();
    for (i = 0; i < vertexNum; ++i) {
        if (graph[0][i])
            dist[i] = graph[0][i];
    }

    totalWeight = 0; vCount = 0;

    dist[0] = 0;
    ++vCount;

    while (1) {
        v = findMinDist();
        if (v == -1)
            break;

        totalWeight += dist[v];
        dist[v] = 0;
        ++vCount;

        for (i = 0; i < vertexNum; ++i)
        if (dist[i] != 0 && graph[v][i] && graph[v][i] < dist[i]) {
            dist[i] = graph[v][i];
        }
    }
    if (vCount < vertexNum)
        totalWeight = -1;
    return totalWeight;
}
