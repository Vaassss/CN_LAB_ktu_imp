#include <stdio.h>

struct node {
    unsigned dist[20];
    unsigned from[20];
} n[10];

int main() {
    int costmat[20][20];
    int nodes, i, j, k, count = 0;

    printf("Enter the number of nodes: ");
    scanf("%d", &nodes);

    printf("Enter the cost matrix:\n");
    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            scanf("%d", &costmat[i][j]);
            n[i].dist[j] = costmat[i][j];
            n[i].from[j] = j;
        }
    }

    do {
        count = 0;
        for (i = 0; i < nodes; i++) {
            for (j = 0; j < nodes; j++) {
                for (k = 0; k < nodes; k++) {
                    if (n[i].dist[j] > costmat[i][k] + n[k].dist[j]) {
                        n[i].dist[j] = costmat[i][k] + n[k].dist[j];
                        n[i].from[j] = k;
                        count++;
                    }
                }
            }
        }
    } while (count != 0);

    for (i = 0; i < nodes; i++) {
        printf("For router %d:\n", i + 1);
        for (j = 0; j < nodes; j++) {
            printf("Node %d via %d, distance %d\n", j + 1, n[i].from[j] + 1, n[i].dist[j]);
        }
    }
    printf("\n");

    return 0;
}
