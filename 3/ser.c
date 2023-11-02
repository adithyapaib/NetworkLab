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
    int sockfd, ret;                                            
    struct sockaddr_in serverAddr;                              
    int clientSocket;                                           

    struct sockaddr_in cliAddr;                                 

    socklen_t addr_size;                                        

    pid_t childpid;                                             

    sockfd = socket(AF_INET, SOCK_STREAM, 0);                   

    if (sockfd < 0) {                                           
        printf("Error in connection.");
        exit(1);
    }

    printf("Server Socket is created.");
    memset(&serverAddr, '\0', sizeof(serverAddr));              
    serverAddr.sin_family = AF_INET;                            
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");         

    ret = bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)); 

    if (ret < 0) {                                              
        printf("Error in binding.");
        exit(1);
    }

    if (listen(sockfd, 10) == 0)                               
    {
        printf("Listening...");
    }

    int cnt = 0;
    while (1) {
        clientSocket = accept(sockfd, (struct sockaddr*)&cliAddr,&addr_size);  

        if (clientSocket < 0)                              
        {
            exit(1);
        }

        printf("Connection accepted from %s:%d",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));  
        printf("Clients connected: %d",++cnt);        

        if ((childpid = fork()) == 0)                     
        {
            close(sockfd);                            
            printf("Child process: PID=%d, PPID=%d", getpid(), getppid());  
            send(clientSocket, "hi client", strlen("hi client"), 0);  
            close(clientSocket);                                     
            exit(0);  
        }
        else if (childpid > 0) {
            printf("Parent process: PID=%d, PPID=%d", getpid(), getppid());  
            close(clientSocket);                                     
        }
        else {
            printf("Error in creating child process.\n");
        }
    }
    close(sockfd);                                     
    return 0;
}