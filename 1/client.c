#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 4096
#define SERVER_PORT 3566

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    // Set server address and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error in connecting to server");
        exit(1);
    }

    // Send and receive messages with the server
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
            perror("Error in receiving message");
            exit(1);
        }
        printf("%s", buffer);

        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; 

        if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
            perror("Error in sending message");
            exit(1);
        }

        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    close(clientSocket); // Close the socket

    return 0;
}