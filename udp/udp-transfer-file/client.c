#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define FILENAME "receivefile.txt"

int main() {
  int fd;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in server;
  FILE *file;
  socklen_t addr_len = sizeof(server);

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = INADDR_ANY;

  strcpy(buffer, "REQUEST_FILE");
  sendto(fd, buffer, strlen(buffer), 0,
         (const struct sockaddr *)&server, addr_len);
  printf("Request sent to server.\n");

  file = fopen(FILENAME, "wb");
  if (file == NULL) {
    perror("Failed to open file");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("Receiving file...\n");
  while (1) {
    memset(buffer, 0, BUFFER_SIZE);
    int n = recvfrom(fd, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (n < 0) {
      perror("Receive failed");
      break;
    }

    buffer[n] = '\0';

    if (strcmp(buffer, "EOF") == 0) {
      printf("File transfer complete.\n");
      break;
    }

    fwrite(buffer, 1, n, file);
  }

  fclose(file);
  close(fd);
  return 0;
}
