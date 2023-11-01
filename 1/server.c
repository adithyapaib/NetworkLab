#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX 1024
#define MAX_ELEMENTS 100

char buff[MAX];

// Clear the buffer
void clearBuffer() {
    memset(buff, '\0', sizeof(buff));
}

// Send a message to the client
void sendMsg(int clientSocket, char *msg) {
    clearBuffer();
    strcpy(buff, msg);
    if (send(clientSocket, buff, strlen(buff), 0) < 0) {
        perror("Error in sending message");
        exit(1);
    }
}

// Receive a message from the client
void recvMsg(int clientSocket) {
    clearBuffer();
    if (recv(clientSocket, buff, sizeof(buff), 0) < 0) {
        perror("Error in receiving message");
        exit(1);
    }
    printf("%s", buff);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[MAX];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    // Set server address and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(3566);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    // Listen for connection
    if (listen(serverSocket, 5) < 0) {
        perror("Error in listening");
        exit(1);
    }
    printf("Server listening...\n");

    // Accept connections from clients
    addr_size = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_size);
    if (clientSocket < 0) {
        perror("Error in accepting");
        exit(1);
    }

    while (1) {
        char c ;
        // Write a message to the client to enter a choice of operation
        clearBuffer();
        sendMsg(clientSocket, "\nEnter your choice:\n1) Search\n2) Sort\n3) Split\n4) Exit: ");
        recvMsg(clientSocket);
        char choice = buff[0];
        clearBuffer();

        if (choice == '4')
            break;

        sendMsg(clientSocket, "Enter the number of elements: ");
        recvMsg(clientSocket);


        int num_elements = atoi(buff);
        printf("Number of elements: %d\n", num_elements);

        int elements[MAX_ELEMENTS];

        // Send a message to enter the elements
        sendMsg(clientSocket, "Enter elements (space-separated): ");

        // Receive the elements from the client
        recvMsg(clientSocket);

        // Parse the elements from the buffer
        char *token = strtok(buff, " ");
        int i = 0;
        while (token != NULL) {
            elements[i++] = atoi(token);
            token = strtok(NULL, " ");
        }
        printf("\n");

        switch (choice) {
            case '1':
                // Handle the "Search" operation here
                sendMsg(clientSocket, "Enter the element to search: ");
                recvMsg(clientSocket);
                int search_element = atoi(buff);
                int found = 0;
                for (int i = 0; i < num_elements; i++) {
                    if (elements[i] == search_element) {
                        found = i;
                        break;
                    }
                }
                if (found) {
                    sprintf(buff, "Element found at index %d\n", found);
                    sendMsg(clientSocket, buff);
                } else {
                    sendMsg(clientSocket, "Element not found\n");
                }
                break;

            case '2':
                // Sort the elements and send them back to the client
                for (int i = 0; i < num_elements; i++) {
                    for (int j = i + 1; j < num_elements; j++) {
                        if (elements[i] > elements[j]) {
                            int temp = elements[i];
                            elements[i] = elements[j];
                            elements[j] = temp;
                        }
                    }
                }
                clearBuffer();
                for (int i = 0; i < num_elements; i++) {
                    sprintf(buff, "%d ", elements[i]);
                    sendMsg(clientSocket, buff);
                }
                sendMsg(clientSocket, "\n");
                break;

            case '3':
                
                sendMsg(clientSocket, "Even elements: ");
                clearBuffer();
                strcpy(buff, "\n Even elements: ");
                for (int i = 0; i < num_elements; i++) {
                    if (elements[i] % 2 == 0)
                    {
                        buff[strlen(buff)] = elements[i] + '0';
                        printf("%d ", elements[i]);
                       
                    }
                     sendMsg(clientSocket, buff);

                        

                       
                    
                }clearBuffer();

                strcpy(buff, "\n Odd elements: ");


                for (int i = 0; i < num_elements; i++) {
                    if (elements[i] % 2 != 0) {
                        buff[strlen(buff)] = elements[i] + '0';
                        printf("%d ", elements[i]);

                    }
                    sendMsg(clientSocket, buff);
                }
                break;

            default:
                sendMsg(clientSocket, "\nInvalid choice\n");
                break;
        }
        clearBuffer();
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}