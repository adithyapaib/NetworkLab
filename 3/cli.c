#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 4444

int main()
{
    int clientSocket, ret;
    struct sockaddr_in cliAddr, serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket < 0)
    {
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Client Socket is created.\n");

    memset(&cliAddr, '\0', sizeof(cliAddr));
    memset(buffer, '\0', sizeof(buffer));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (ret < 0)
    {
        printf("Error in connection.\n");
        exit(1);
    }

    printf("Connected to Server.\n");

    while (1)
    {
        if (recv(clientSocket, buffer, 1024, 0) < 0)
        {
            printf("Error in receiving data.\n");
        }
        else
        {
            printf("Server: %s\n", buffer);
            bzero(buffer, sizeof(buffer));
        }
    }

    return 0;
}