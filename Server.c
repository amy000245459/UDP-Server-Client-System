#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
int main(int argc, char **argv){
 
    if(argc != 3){
    printf("Usage: %s <IP> <port>\n", argv[0]);
    exit(1);
  }
 
  char *ip = argv[1];
  int port = atoi(argv[2]);
 
  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];
  socklen_t addr_size;
 
  // Create socket
  // socket(domain, type, protocol) 
  // for udp, only AF_INET, SOCK_DGRAM and no variant > protocol =0
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("[-]socket error");
    exit(1);
  }
  
  //Bind the socket with the proper IP (Internet Protocol) adress and the port number
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);
 
  if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
    perror("[-]bind error");
    exit(1);
  }
  
  bzero(buffer, 1024);
  addr_size = sizeof(client_addr);
  recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_size);
  // Echo back the message from client
  sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, addr_size);
 
  return 0;
}