#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8888
int v[5] = {0, 0, 0, 0, 0};



int countVowels(char *str)
{

    int count = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        switch (str[i])
        {
        case 'a':
            v[0]++;
            count++;
            break;
        case 'e':
            v[1]++;
            count++;
            break;
        case 'i':
            v[2]++;
            count++;
            break;
        case 'o':
            v[3]++;
            count++;
            break;
        case 'u':
            v[4]++;
            count++;
            break;
        }
    }
}

void pali(char *str, char *res)
{
    // check if the string is palindrome and also send the length of the string
    memset(res, '\0', sizeof(res));
    char t[100];
    strcpy(t, str);
    for (int i = 0; i < strlen(str); i++)
        str[i] = t[strlen(str) - i - 1];
    if (strcmp(t, str) == 0)
        sprintf(res, "The string is palindrome");
    else
        sprintf(res, "The string is not palindrome");
    sprintf(res, "%s\nLength of the string: %d", res, strlen(str));
}

int main()
{
    char res[1024];
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Error in socket");
        exit(1);
    }

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Error in bind");
        exit(1);
    }

    printf("UDP server is listening on port %d...\n", SERVER_PORT);

    while (1)
    {
        socklen_t addrLen = sizeof(clientAddr);

        // Receive message from client
        int recvLen = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (recvLen < 0)
        {
            perror("Error in recvfrom");
            exit(1);
        }

        buffer[recvLen] = '\0';

        // Check if the message is "halt"
        if (strcmp(buffer, "halt") == 0)
        {
            printf("Server is stopping...\n");
            break;
        }
        pali(buffer, res);
        countVowels(buffer);
        char vc[100];
        sprintf(vc, "%s \nVowels count: a = %d, e = %d, i = %d, o = %d, u = %d",res, v[0], v[1], v[2], v[3], v[4]);

        sendto(sockfd, vc, strlen(vc), 0, (struct sockaddr *)&clientAddr, addrLen);
    }

    // Close the socket
    close(sockfd);

    return 0;
}