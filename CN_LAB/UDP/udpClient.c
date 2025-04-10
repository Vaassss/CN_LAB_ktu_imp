#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUF_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, from_addr;
    char buffer[BUF_SIZE];
    socklen_t from_len = sizeof(from_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter message to send to server: ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Strip newline

    if (strlen(buffer) == 0)
    {
        printf("No message entered. Exiting.\n");
        close(sockfd);
        return 1;
    }

    if (sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("sendto failed");
        close(sockfd);
        return 1;
    }

    printf("Message sent to server\n");

    int n = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0,
                     (struct sockaddr *)&from_addr, &from_len);
    if (n < 0)
    {
        perror("recvfrom failed");
        close(sockfd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Server response: %s\n", buffer);

    close(sockfd);
    return 0;
}
