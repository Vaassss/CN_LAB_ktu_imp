#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main() 
{
    printf("Server side:\n");

    char tcpbuffer[1024];
    int sockfd, newsocket;
    struct sockaddr_in addr1, addr2;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Setup address structure
    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = INADDR_ANY;
    addr1.sin_port = htons(5000); // Always use htons

    // Bind
    if (bind(sockfd, (struct sockaddr*)&addr1, sizeof(addr1)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    // Listen
    listen(sockfd, 5);
    printf("Waiting for connection...\n");

    // Accept connection
    newsocket = accept(sockfd, (struct sockaddr*)&addr2, &addrlen);
    if (newsocket < 0) {
        perror("Accept failed");
        close(sockfd);
        exit(1);
    }

    printf("Connection established!\n");

    while (1) {
        printf("Receiving message from client...\n");

        int n = recv(newsocket, tcpbuffer, sizeof(tcpbuffer) - 1, 0);
        if (n <= 0) break;

        tcpbuffer[n] = '\0'; // Null-terminate received string
        printf("Client: %s\n", tcpbuffer);

        if (strcmp(tcpbuffer, "stop") == 0)
            break;

        printf("Enter the message: ");
        fgets(tcpbuffer, sizeof(tcpbuffer), stdin);

        // Remove trailing newline from fgets
        tcpbuffer[strcspn(tcpbuffer, "\n")] = '\0';

        send(newsocket, tcpbuffer, strlen(tcpbuffer), 0);

        if (strcmp(tcpbuffer, "stop") == 0)
            break;
    }

    close(newsocket);
    close(sockfd);

    return 0;
}
