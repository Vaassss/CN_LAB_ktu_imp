#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main() 
{
    printf("Client side:\n");

    char tcpbuffer[1024];
    int sockfd;
    struct sockaddr_in addr1;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Setup server address
    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(5000);
    addr1.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with actual server IP if needed

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&addr1, sizeof(addr1)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    while (1) {
        printf("Sending message to server: ");
        fgets(tcpbuffer, sizeof(tcpbuffer), stdin);

        // Remove trailing newline from fgets
        tcpbuffer[strcspn(tcpbuffer, "\n")] = '\0';

        send(sockfd, tcpbuffer, strlen(tcpbuffer), 0);

        if (strcmp(tcpbuffer, "stop") == 0)
            break;

        int n = recv(sockfd, tcpbuffer, sizeof(tcpbuffer) - 1, 0);
        if (n <= 0) {
            printf("Disconnected by server.\n");
            break;
        }

        tcpbuffer[n] = '\0';
        printf("Received from server: %s\n", tcpbuffer);

        if (strcmp(tcpbuffer, "stop") == 0)
            break;
    }

    close(sockfd);
    return 0;
}
