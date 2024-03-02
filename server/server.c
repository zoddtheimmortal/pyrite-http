#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_BUFF 1024
#define PORT 8080
const char ADDR[] = "172.16.29.110";

int main()
{
    int sfd, cfd;
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

    if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Bind failed\n");
        return -1;
    }
    else
    {
        printf("Bind successful\n");
    }

    listen(sfd, 5);

    cfd = accept(sfd, NULL, NULL);
    printf("Connection established...\n");

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds); // for setting stdin
        FD_SET(cfd, &readfds);

        select(cfd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(0, &readfds))
        {
            printf("Server: \n");
            scanf("%s", buff);
            write(cfd, buff, strlen(buff));
        }

        if (FD_ISSET(cfd, &readfds))
        {
            read(cfd, buff, sizeof(buff));
            if (strcmp(buff, "exit"))
                break;

            printf("Client: %s\n", buff);
        }
    }

    close(sfd);
    close(cfd);
}