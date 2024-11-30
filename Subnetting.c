// Broadcast Tree
/*
Take an example subnet of hosts and obtain a broadcast tree for the subnet.

Sample output:

Enter IP address (e.g., 192.168.1.0): 192.168.1.0
Enter current prefix length (e.g., 24): 24

Number of subnets: 2
Number of hosts per subnet: 126

Subnet 1:
Network Address: 192.168.1.0
Broadcast Address: 192.168.1.127
Subnet Mask: 255.255.255.128
First Host: 192.168.1.1
Last Host: 192.168.1.126

Subnet 2:
Network Address: 192.168.1.128
Broadcast Address: 192.168.1.255
Subnet Mask: 255.255.255.128
First Host: 192.168.1.129
Last Host: 192.168.1.254

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <math.h>

void calculate_subnets(const char *ip_address, int prefix_length) {
  unsigned int num_subnets = 2;
  int hosts_per_subnet = (1 << (32 - (prefix_length + 1))) - 2;
  unsigned int ip, mask, subnet_mask, subnet_bits, new_prefix_length;
  struct in_addr addr;
  char subnet_ip[INET_ADDRSTRLEN], broadcast_ip[INET_ADDRSTRLEN],
      subnet_mask_ip[INET_ADDRSTRLEN];

  inet_pton(AF_INET, ip_address, &ip);
  subnet_bits = (int) ceil(log2(num_subnets));
  new_prefix_length = prefix_length + subnet_bits;
  subnet_mask = htonl(0xFFFFFFFF << (32 - new_prefix_length));
  addr.s_addr = subnet_mask;
  inet_ntop(AF_INET, &addr, subnet_mask_ip, INET_ADDRSTRLEN);
  printf("Number of subnets: %d\n", num_subnets);
  printf("Number of hosts per subnet: %d\n\n", hosts_per_subnet);

  for (int i = 0; i < num_subnets; i++) {
    unsigned int subnet = (ntohl(ip) & (0xFFFFFFFF << (32 - prefix_length)))
        | (i << (32 - new_prefix_length));
    unsigned int broadcast = subnet | ~(ntohl(subnet_mask));

    addr.s_addr = htonl(subnet);
    inet_ntop(AF_INET, &addr, subnet_ip, INET_ADDRSTRLEN);
    addr.s_addr = htonl(broadcast);
    inet_ntop(AF_INET, &addr, broadcast_ip, INET_ADDRSTRLEN);

    printf("Subnet %d:\n", i + 1);
    printf("Network Address: %s\n", subnet_ip);
    printf("Broadcast Address: %s\n", broadcast_ip);
    printf("Subnet Mask: %s\n", subnet_mask_ip);

    addr.s_addr = htonl(subnet + 1);
    printf("First Host: %s\n", inet_ntoa(addr));

    addr.s_addr = htonl(broadcast - 1);
    printf("Last Host: %s\n", inet_ntoa(addr));
    printf("\n");
  }
}

int main() {
  char ip_address[INET_ADDRSTRLEN];
  int prefix_length, num_subnets, hosts_per_subnet;

  printf("Enter IP address (e.g., 192.168.1.0): ");
  scanf("%s", ip_address);
  printf("Enter current prefix length (e.g., 24): ");
  scanf("%d", &prefix_length);
  printf("\n");
  calculate_subnets(ip_address, prefix_length);
  return 0;
}