#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void sortNumbers(char *str)
{
    int len = strlen(str);
    int numbers[len];
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        if (isdigit(str[i]))
        {
            numbers[count++] = str[i] - '0';
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (numbers[j] > numbers[j + 1])
            {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }

    count = 0;
    for (int i = 0; i < len; i++)
    {
        if (isdigit(str[i]))
        {
            str[i] = numbers[count++] + '0';
        }
    }
}

void sortCharacters(char *str)
{
    int len = strlen(str);

    for (int i = 0; i < len - 1; i++)
    {
        for (int j = 0; j < len - i - 1; j++)
        {
            if (str[j] < str[j + 1])
            {
                char temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {

        valread = read(new_socket, buffer, 1024);
        printf("Received alphanumeric string from client: %s\n", buffer);

        sortNumbers(buffer);
        printf("Sorted numbers in ascending order: %s\n", buffer);

        char result[1024];
        sprintf(result, "Child process ID: %d, Sorted numbers: %s", getpid(), buffer);
        send(new_socket, result, strlen(result), 0);
    }
    else
    {

        valread = read(new_socket, buffer, 1024);
        printf("Received alphanumeric string from client: %s\n", buffer);

        sortCharacters(buffer);
        printf("Sorted characters in descending order: %s\n", buffer);

        char result[1024];
        sprintf(result, "Parent process ID: %d, Sorted characters: %s", getpid(), buffer);
        send(new_socket, result, strlen(result), 0);
    }

    return 0;
}