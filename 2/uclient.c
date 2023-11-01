#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket");
        exit(1);
    }

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr)) <= 0) {
        perror("Error in inet_pton");
        exit(1);
    }

    while (1) {
        printf("Enter a string (or 'halt' to stop): ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        // Remove newline character from the input
        buffer[strcspn(buffer, "\n")] = '\0';

        // Send message to server
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("Error in sendto");
            exit(1);
        }

        // Check if the message is "halt"
        if (strcmp(buffer, "halt") == 0) {
            printf("Client is stopping...\n");
            break;
        }

        // Receive response from server
        int recvLen = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, NULL, NULL);
        if (recvLen < 0) {
            perror("Error in recvfrom");
            exit(1);
        }

        buffer[recvLen] = '\0';

        // Print the response
        printf("%s\n", buffer);
    }

    // Close the socket
    close(sockfd);

    return 0;
}