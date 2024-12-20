#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#define BUFSIZE 512
#define MYPORT 11710
#define MAXNAME 100

void reverse_string(char *str, int length)
{
    int start = 0, end = length - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int main(int argc, char **argv)
{
    int sd, n, ret;
    struct sockaddr_in serveraddress, cliaddr;
    socklen_t length;
    char clientname[MAXNAME], datareceived[BUFSIZE];

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("socket");
        exit(1);
    }

    memset(&serveraddress, 0, sizeof(serveraddress));
    memset(&cliaddr, 0, sizeof(cliaddr));
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(MYPORT);
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(sd, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (ret < 0)
    {
        perror("BIND FAILS");
        exit(1);
    }

    for (;;)
    {
        printf("I am waiting\n");
        length = sizeof(cliaddr);
        n = recvfrom(sd, datareceived, BUFSIZE, 0, (struct sockaddr *)&cliaddr, &length);
        if (n > 0)
        {
            datareceived[n] = '\0';
            printf("Data Received from %s\n", inet_ntop(AF_INET, &cliaddr.sin_addr, clientname, sizeof(clientname)));
            printf("I have received: %s\n", datareceived);
            reverse_string(datareceived, n);
            printf("Sending reversed string: %s\n", datareceived);
            sendto(sd, datareceived, n, 0, (struct sockaddr *)&cliaddr, length);
        }
    }
}
