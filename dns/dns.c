#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    struct hostent *hen;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    hen = gethostbyname(argv[1]);
    if (hen == NULL)
    {
        fprintf(stderr, "Error: Host not found for %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("Hostname: %s\n", hen->h_name);
    printf("IP Address: %s\n", inet_ntoa(*((struct in_addr *)hen->h_addr)));

    return EXIT_SUCCESS;
}
