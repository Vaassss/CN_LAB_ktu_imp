#include <stdio.h>

int main() {
    int incoming, outgoing, bsize, n, store = 0;

    printf("Enter the bucket size: ");
    scanf("%d", &bsize);

    printf("Enter the outgoing rate: ");
    scanf("%d", &outgoing);

    printf("Enter the number of inputs: ");
    scanf("%d", &n);

    while (n != 0) {
        printf("Enter the incoming packet size: ");
        scanf("%d", &incoming);

        printf("The incoming packet size is %d\n", incoming);

        if (incoming <= (bsize - store)) {
            store += incoming;
            printf("Bucket buffer size %d out of %d\n", store, bsize);
        } else {
            printf("Dropped %d number of packets\n", incoming - (bsize - store));
            printf("Bucket buffer size %d out of %d\n", store, bsize);
            store = bsize;
        }

        if (store >= outgoing) {
            store -= outgoing;
        } else {
            store = 0;
        }

        printf("After outgoing, %d packets left out of %d in buffer\n", store, bsize);
        n--;
    }

    return 0;
}
