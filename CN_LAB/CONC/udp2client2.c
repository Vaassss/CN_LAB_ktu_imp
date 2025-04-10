#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
    printf("Client 1 Side !!!\n");

    char buffer[50];
    int sockfd;
    struct sockaddr_in addr;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(5000);

    socklen_t s = sizeof(struct sockaddr_in);

    do {
        printf("Enter message for server: ");
        scanf("%s", buffer);

        // Break the loop if "stop" is entered
        if (strcmp(buffer, "stop") == 0)
            break;

        // Send message to the server
        sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&addr, s);

        // Receive response from the server
        recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&addr, &s);

        // Display the response from the server
        printf("Received from server: %s\n", buffer);
    } while (1);

    // Close the socket
    close(sockfd);

    return 0;
}
