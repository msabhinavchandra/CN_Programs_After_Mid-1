#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1025];
    int numrv;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        printf("Error: Could not create socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Error: Bind failed\n");
        return -1;
    }

    if (listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }

    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
        if (connfd < 0)
        {
            printf("Error: Accept failed\n");
            continue;
        }

        FILE *fp = fopen("testfile.txt", "rb");
        if (fp == NULL)
        {
            printf("Error: File open error\n");
            close(connfd);
            continue;
        }
        while (1)
        {
            unsigned char buff[256] = {0};
            int nread = fread(buff, 1, 256, fp);

            printf("Bytes read %d \n", nread);
            if (nread > 0)
            {
                printf("Sending data...\n");
                write(connfd, buff, nread);
            }
            if (nread < 256)
            {
                if (feof(fp))
                {
                    printf("End of file reached\n");
                }
                if (ferror(fp))
                {
                    printf("Error reading the file\n");
                }
                break;
            }
        }
        fclose(fp);
        close(connfd);
        printf("File transfer completed. Connection closed.\n");
    }
    return 0;
}
