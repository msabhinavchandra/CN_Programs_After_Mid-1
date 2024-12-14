#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE 100
#define SERV_PORT 13154
#define OPEN_MAX 5

int main(int argc, char **argv) {
    int k, i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket error");
        exit(1);
    }
    printf("Socket Created");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
        close(listenfd);
        exit(1);
    }

    if (listen(listenfd, 5) < 0) {
        perror("listen error");
        close(listenfd);
        exit(1);
    }

    client[0].fd = listenfd;
    client[0].events = POLLIN;
    for (i = 1; i < OPEN_MAX; i++) {
        client[i].fd = -1;
    }

    maxi = 0;

    for (;;) {
        nready = poll(client, maxi + 1, -1);
        if (nready < 0) {
            perror("poll error");
            exit(1);
        }

        if (client[0].revents & POLLIN) {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
            if (connfd < 0) {
                perror("accept error");
                continue;
            }

            for (i = 1; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            }

            if (i == OPEN_MAX) {
                printf("Too many clients\n");
                close(connfd);
                continue;
            }

            client[i].events = POLLIN;
            if (i > maxi) {
                maxi = i;
            }

            if (--nready <= 0)
                continue;
        }

        for (i = 1; i <= maxi; i++) {
            if ((sockfd = client[i].fd) < 0)
                continue;

            if (client[i].revents & (POLLIN | POLLERR)) {
                if ((n = read(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        client[i].fd = -1;
                    } else {
                        perror("read error");
                    }
                } else if (n == 0) {
                    close(sockfd);
                    client[i].fd = -1;
                } else {
                    line[n] = '\0'; // Ensure null-terminated string
                    printf("Data from client: %s\n", line);
                }

                if (--nready <= 0)
                    break;
            }
        }
    }
}
