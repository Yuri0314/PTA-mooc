#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRINGLENGTH 1000000
#define PATTERNLENGTH 100000

typedef int Position;
#define NotFound -1

void BuildMatch(char *pattern, int *match);
Position KMP(char *string, char *pattern);

int main()
{
    char string[STRINGLENGTH + 1];
    char pattern[PATTERNLENGTH + 1];
    int N, i;
    Position p;
    scanf("%s", string);
    scanf("%d", &N);

    for (i = 0; i < N; ++i) {
        scanf("%s", pattern);
        p = KMP(string, pattern);
        if (p == NotFound) printf("Not Found\n");
        else printf("%s\n", string + p);
    }

    return 0;
}

void BuildMatch(char *pattern, int *match)
{
    Position i, j;
    int m = strlen(pattern);
    match[0] = -1;

    for (j = 1; j < m; ++j) {
        i = match[j - 1];
        while ((i >= 0) && (pattern[i + 1] != pattern[j]))
            i = match[i];
        if (pattern[i + 1] == pattern[j])
            match[j] = i + 1;
        else match[j] = -1;
    }
}

Position KMP(char *string, char *pattern)
{
    int n = strlen(string);
    int m = strlen(pattern);
    Position s, p, *match;

    if (n < m) return NotFound;
    match = (Position *)malloc(sizeof(Position) * m);
    BuildMatch(pattern, match);
    s = p = 0;
    while (s < n && p < m) {
        if (string[s] == pattern[p]) {
            ++s; ++p;
        }
        else if (p > 0) p = match[p - 1] + 1;
        else ++s;
    }
    free(match);
    return (p == m) ? (s - m) : NotFound;
}
