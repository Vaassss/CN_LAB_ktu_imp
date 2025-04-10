#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

int main() {
    printf("Server Side !!!\n");

    char buffer[50];
    int sockfd;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(5000);

    socklen_t s = sizeof(struct sockaddr_in);

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Bind the socket to the specified address and port
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return -1;
    }

    printf("Connection established...\n");

    do {
        // Receive message from the client
        recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&addr, &s);
        printf("Message from client: %s\n", buffer);

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            break;
        }

        if (pid == 0) {
            // Child process: send the current time back to the client
            char timeBuffer[50];
            time_t rawtime;
            struct tm *timeinfo;

            time(&rawtime);
            timeinfo = localtime(&rawtime);

            sprintf(timeBuffer, "Time is %02d:%02d:%02d",
                    timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

            sendto(sockfd, timeBuffer, strlen(timeBuffer) + 1, 0, (struct sockaddr *)&addr, s);
            printf("Time sent to client...\n");

            // Exit the child process
            _exit(0);
        }
    } while (strcmp(buffer, "stop") != 0);

    // Close the socket
    close(sockfd);

    return 0;
}
