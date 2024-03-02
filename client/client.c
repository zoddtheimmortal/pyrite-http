#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_BUFF 1024
#define PORT 8080
const char ADDR[] = "172.16.129.110";

int main()
{
    int sfd;
    fd_set readfds;
    char buff[MAX_BUFF] = "";
    struct sockaddr_in server_addr;

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sfd < 0)
    {
        printf("Socket creation failed\n");
        return -1;
    }
    else
    {
        printf("Socket created\n");
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_aton(ADDR, &server_addr.sin_addr);

    printf("Connecting to server: %d\n", connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)));

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds); // for setting stdin
        FD_SET(sfd, &readfds);

        select(sfd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(0, &readfds))
        {
            read(0, buff, MAX_BUFF);
            write(sfd, buff, strlen(buff));
            if (strcmp(buff, "exit\n") == 0)
            {
                printf("Closing the connection\n");
                break;
            }
        }

        if (FD_ISSET(sfd, &readfds))
        {
            read(sfd, buff, MAX_BUFF);
            if (strcmp(buff, "exit") == 0)
            {
                printf("Server closed the connection\n");
                break;
            }
            else
            {
                printf("Server: %s\n", buff);
            }
        }
    }
    close(sfd);
}