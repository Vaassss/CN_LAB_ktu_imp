#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8090

int main()
{
    struct sockaddr_in serv_addr;
    int sock = 0, valread;
    char str[100];
    char buffer[1024] = {0};

    // Input string from user
    printf("Input the string: ");
    scanf(" %[^\n]", str); // Note the space before %[^\n] to skip leading whitespace

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and connect
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // Send only the meaningful bytes
    int len = strlen(str);
    send(sock, str, len, 0);

    // Receive reversed string
    valread = read(sock, buffer, sizeof(buffer) - 1);
    if (valread < 0) {
        perror("Read error");
        close(sock);
        return -1;
    }

    buffer[valread] = '\0'; // Null-terminate received data
    printf("Reversed string from server: %s\n", buffer);

    close(sock);
    return 0;
}

