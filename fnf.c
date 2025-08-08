#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10

char productions[MAX][10];
char first[MAX][10], follow[MAX][10];
int numProductions;

int isNonTerminal(char ch) {
    return ch >= 'A' && ch <= 'Z';
}

int isPresent(char *arr, char ch) {
    for (int i = 0; arr[i] != '\0'; i++)
        if (arr[i] == ch)
            return 1;
    return 0;
}

void findFirst(char ch, int index);

void findFollow(char ch);

void addFirstToFirst(char *to, char *from) {
    for (int i = 0; from[i] != '\0'; i++) {
        if (!isPresent(to, from[i])) {
            int len = strlen(to);
            to[len] = from[i];
            to[len + 1] = '\0';
        }
    }
}

void computeFirst() {
    for (int i = 0; i < numProductions; i++) {
        char ch = productions[i][0];
        findFirst(ch, i);
    }
}

void findFirst(char ch, int index) {
    // If first already computed, skip
    if (strlen(first[index]) > 0)
        return;

    for (int i = 0; i < numProductions; i++) {
        if (productions[i][0] == ch) {
            // Production RHS start
            char *rhs = productions[i] + 2;

            // If epsilon production
            if (rhs[0] == '#') {
                if (!isPresent(first[index], '#')) {
                    int len = strlen(first[index]);
                    first[index][len] = '#';
                    first[index][len + 1] = '\0';
                }
                continue;
            }

            // Check symbols in RHS
            for (int j = 0; rhs[j] != '\0'; j++) {
                if (!isNonTerminal(rhs[j])) {
                    // Terminal
                    if (!isPresent(first[index], rhs[j])) {
                        int len = strlen(first[index]);
                        first[index][len] = rhs[j];
                        first[index][len + 1] = '\0';
                    }
                    break;
                } else {
                    // Non-terminal
                    int pos = -1;
                    for (int k = 0; k < numProductions; k++) {
                        if (productions[k][0] == rhs[j]) {
                            pos = k;
                            break;
                        }
                    }
                    if (pos == -1)
                        break;

                    findFirst(rhs[j], pos);

                    // Add First(rhs[j]) except epsilon
                    for (int k = 0; first[pos][k] != '\0'; k++) {
                        if (first[pos][k] != '#' && !isPresent(first[index], first[pos][k])) {
                            int len = strlen(first[index]);
                            first[index][len] = first[pos][k];
                            first[index][len + 1] = '\0';
                        }
                    }

                    // If epsilon not in First(rhs[j]), break
                    if (!isPresent(first[pos], '#')) {
                        break;
                    } else if (rhs[j + 1] == '\0') {
                        // If epsilon in last symbol's First, add epsilon to First(ch)
                        if (!isPresent(first[index], '#')) {
                            int len = strlen(first[index]);
                            first[index][len] = '#';
                            first[index][len + 1] = '\0';
                        }
                    }
                }
            }
        }
    }
}

void computeFollow() {
    // Start symbol's Follow gets '$'
    follow[0][0] = '$';
    follow[0][1] = '\0';

    for (int i = 0; i < numProductions; i++) {
        findFollow(productions[i][0]);
    }
}

void findFollow(char ch) {
    for (int i = 0; i < numProductions; i++) {
        char *rhs = productions[i] + 2;
        for (int j = 0; rhs[j] != '\0'; j++) {
            if (rhs[j] == ch) {
                if (rhs[j + 1] != '\0') {
                    if (!isNonTerminal(rhs[j + 1])) {
                        // Terminal next to ch
                        int pos = -1;
                        for (int k = 0; k < numProductions; k++) {
                            if (productions[k][0] == ch) pos = k;
                        }
                        if (pos != -1 && !isPresent(follow[pos], rhs[j + 1])) {
                            int len = strlen(follow[pos]);
                            follow[pos][len] = rhs[j + 1];
                            follow[pos][len + 1] = '\0';
                        }
                    } else {
                        // Non-terminal next to ch
                        int posNext = -1;
                        for (int k = 0; k < numProductions; k++) {
                            if (productions[k][0] == rhs[j + 1]) posNext = k;
                        }
                        if (posNext != -1) {
                            int posCh = -1;
                            for (int k = 0; k < numProductions; k++) {
                                if (productions[k][0] == ch) posCh = k;
                            }

                            // Add First(rhs[j+1]) except epsilon to Follow(ch)
                            for (int k = 0; first[posNext][k] != '\0'; k++) {
                                if (first[posNext][k] != '#' && !isPresent(follow[posCh], first[posNext][k])) {
                                    int len = strlen(follow[posCh]);
                                    follow[posCh][len] = first[posNext][k];
                                    follow[posCh][len + 1] = '\0';
                                }
                            }

                            // If epsilon in First(rhs[j+1]), add Follow(lhs) to Follow(ch)
                            if (isPresent(first[posNext], '#')) {
                                if (productions[i][0] != ch) {
                                    findFollow(productions[i][0]);
                                    int posLhs = -1;
                                    for (int k = 0; k < numProductions; k++) {
                                        if (productions[k][0] == productions[i][0]) posLhs = k;
                                    }
                                    if (posLhs != -1) {
                                        for (int k = 0; follow[posLhs][k] != '\0'; k++) {
                                            if (!isPresent(follow[posCh], follow[posLhs][k])) {
                                                int len = strlen(follow[posCh]);
                                                follow[posCh][len] = follow[posLhs][k];
                                                follow[posCh][len + 1] = '\0';
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else {
                    // ch is at the end, add Follow(lhs) to Follow(ch)
                    if (productions[i][0] != ch) {
                        findFollow(productions[i][0]);
                        int posCh = -1;
                        int posLhs = -1;
                        for (int k = 0; k < numProductions; k++) {
                            if (productions[k][0] == ch) posCh = k;
                            if (productions[k][0] == productions[i][0]) posLhs = k;
                        }
                        if (posCh != -1 && posLhs != -1) {
                            for (int k = 0; follow[posLhs][k] != '\0'; k++) {
                                if (!isPresent(follow[posCh], follow[posLhs][k])) {
                                    int len = strlen(follow[posCh]);
                                    follow[posCh][len] = follow[posLhs][k];
                                    follow[posCh][len + 1] = '\0';
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    FILE *fp = fopen("grammar.txt", "r");
    if (!fp) {
        printf("Error opening file.\n");
        return 1;
    }

    fscanf(fp, "%d", &numProductions);
    for (int i = 0; i < numProductions; i++) {
        fscanf(fp, "%s", productions[i]);
        first[i][0] = '\0';
        follow[i][0] = '\0';
    }

    fclose(fp);

    computeFirst();
    computeFollow();

    printf("First sets:\n");
    for (int i = 0; i < numProductions; i++) {
        printf("First(%c) = { %s }\n", productions[i][0], first[i]);
    }

    printf("\nFollow sets:\n");
    for (int i = 0; i < numProductions; i++) {
        printf("Follow(%c) = { %s }\n", productions[i][0], follow[i]);
    }

    return 0;
}

