#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

// Function to estimate and print all permutations of a string
void generatePermutations(char *str, int left, int right) {
    if (left == right) {
        printf("%s\n", str);
    } else {
        for (int i = left; i <= right; i++) {
            char temp = str[left];
            str[left] = str[i];
            str[i] = temp;
            generatePermutations(str, left + 1, right);
            temp = str[left];
            str[left] = str[i];
            str[i] = temp;
        }
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Error listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for a connection...\n");

    while (1) {
        // Accept a connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Error accepting connection");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        // Receive the string from the client
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Error receiving data");
            close(client_socket);
            continue; // Continue to accept other clients
        }
        buffer[bytes_received] = '\0';

        printf("Received string from the client: %s\n", buffer);

        // Generate and print permutations of the received string
        generatePermutations(buffer, 0, strlen(buffer) - 1);

        // Generate and print permutations of the adjacent string
        int len = strlen(buffer);
        for (int i = 0; i < len - 1; i++) {
            char adjacent[3];
            adjacent[0] = buffer[i];
            adjacent[1] = buffer[i + 1];
            adjacent[2] = '\0';
            generatePermutations(adjacent, 0, 1);
        }

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket (not reached in this code)
    close(server_socket);

    return 0;
}
