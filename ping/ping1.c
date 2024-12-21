#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define ICMP_ECHO 8
#define BUFFER_SIZE 1024

unsigned short calculate_checksum(void *b, int len)
{
  unsigned short *buf = b;
  unsigned int sum = 0;
  unsigned short result;

  for (sum = 0; len > 1; len -= 2)
  {
    sum += *buf++;
  }
  if (len == 1)
  {
    sum += *(unsigned char *)buf;
  }
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  result = ~sum;
  return result;
}

void print_response(struct icmphdr *icmp, struct sockaddr_in *addr,
                    struct timeval *start_time)
{
  struct timeval end_time;
  gettimeofday(&end_time, NULL);

  double elapsed_time = (end_time.tv_sec - start_time->tv_sec) * 1000.0;
  elapsed_time += (end_time.tv_usec - start_time->tv_usec) / 1000.0;

  printf("Reply from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
         inet_ntoa(addr->sin_addr), ntohs(icmp->un.echo.sequence), 64,
         elapsed_time);
}

int resolve_hostname(const char *hostname, char *ip)
{
  struct hostent *host_info;
  struct in_addr **address_list;

  if ((host_info = gethostbyname(hostname)) == NULL)
  {
    herror("gethostbyname");
    return -1;
  }

  address_list = (struct in_addr **)host_info->h_addr_list;
  if (address_list[0] != NULL)
  {
    strcpy(ip, inet_ntoa(*address_list[0]));
    return 0;
  }
  return -1;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <destination IP or hostname>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char *destination = argv[1];
  char destination_ip[BUFFER_SIZE];
  int sockfd;
  struct sockaddr_in addr;
  char send_buffer[BUFFER_SIZE], recv_buffer[BUFFER_SIZE];
  struct icmphdr *icmp;
  struct timeval start_time;

  if (resolve_hostname(destination, destination_ip) != 0)
  {
    fprintf(stderr, "Failed to resolve hostname: %s\n", destination);
    return EXIT_FAILURE;
  }

  printf("PING %s (%s):\n", destination, destination_ip);

  if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
  {
    perror("Socket creation failed");
    return EXIT_FAILURE;
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  if (inet_pton(AF_INET, destination_ip, &addr.sin_addr) <= 0)
  {
    perror("Invalid IP address");
    close(sockfd);
    return EXIT_FAILURE;
  }

  for (int i = 1; i <= 4; i++)
  {
    memset(send_buffer, 0, sizeof(send_buffer));
    icmp = (struct icmphdr *)send_buffer;

    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = getpid();
    icmp->un.echo.sequence = htons(i);
    icmp->checksum = 0;

    icmp->checksum =
        calculate_checksum((unsigned short *)icmp, sizeof(struct icmphdr));

    gettimeofday(&start_time, NULL);
    if (sendto(sockfd, send_buffer, sizeof(struct icmphdr), 0,
               (struct sockaddr *)&addr, sizeof(addr)) <= 0)
    {
      perror("Failed to send packet");
      continue;
    }

    socklen_t addr_len = sizeof(addr);
    if (recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0,
                 (struct sockaddr *)&addr, &addr_len) <= 0)
    {
      perror("Failed to receive packet");
      continue;
    }

    struct icmphdr *recv_icmp =
        (struct icmphdr *)(recv_buffer + sizeof(struct iphdr));

    if (recv_icmp->type == 0)
    {
      print_response(recv_icmp, &addr, &start_time);
    }
    else
    {
      printf("Unexpected ICMP packet type: %d\n", recv_icmp->type);
    }

    sleep(1);
  }

  close(sockfd);
  return EXIT_SUCCESS;
}
