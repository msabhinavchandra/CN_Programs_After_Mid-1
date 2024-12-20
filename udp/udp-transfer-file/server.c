#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define FILENAME "sendfile.txt"

int main() {
  int fd;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in addr, client;
  socklen_t addr_len = sizeof(addr);

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&addr, 0, sizeof(addr));
  memset(&client, 0, sizeof(client));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) <
      0) {
    perror("Bind failed");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("Server is waiting for a request on port %d...\n", PORT);

  recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client,
           &addr_len);
  printf("Received request from client to send file.\n");

  FILE *file;
  file = fopen(FILENAME, "rb");
  if (file == NULL) {
    perror("Failed to open file");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("Sending file...\n");
  while (1) {
    memset(buffer, 0, BUFFER_SIZE);
    int n = fread(buffer, 1, BUFFER_SIZE, file);

    if (n > 0) {
      sendto(fd, buffer, n, 0, (const struct sockaddr *)&client,
             addr_len);
    }

    if (n < BUFFER_SIZE) {
      if (feof(file)) {
        strcpy(buffer, "EOF");
        sendto(fd, buffer, strlen(buffer), 0,
               (const struct sockaddr *)&client, addr_len);
        printf("File transfer complete.\n");
      }
      break;
    }
  }

  fclose(file);
  close(fd);
  return 0;
}
