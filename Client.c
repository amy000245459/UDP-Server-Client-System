#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
// Define fixed variable for waiting interval between retries
const int POWER_BASED = 2;
const int SEC_BASED = 500; // 500ms
const int MAX_INTERVAL=  8000; //8000ms = 8s 
 
int main(int argc, char **argv){
 
  int max_try; 
  if(argc== 4){
    // Setup default value for max_try
    max_try = 1;
  }else if( argc== 5){
    max_try = atoi(argv[4]);
   if(max_try >10){
      printf("Input of retry time excess the maximun(10) \n");
      exit(1);
    }
  }else{
    printf("Usage: %s <IP> <port> <message> <max try(optional)>\n", argv[0]);
    exit(1);
  }
  
  char *ip = argv[1];
  int port = atoi(argv[2]);
  int sockfd, send_respond,i,wait_interval; 
  int multiplier = POWER_BASED;
  struct sockaddr_in serverAddr;
  struct timeval timeout;
  socklen_t addr_size;

  // Setup message
  char buffer[1024];
  bzero(buffer, 1024);
  strcpy(buffer, argv[3] );

  // Setup socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("Socket error");
    exit(1);
  }
  memset(&serverAddr, '\0', sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr(ip);
  
  
  // Send message to server with retry option
  addr_size = sizeof(serverAddr);   
  printf("[Message sent] %s \n",buffer);
  for(i=1; i<= max_try; i++){
    
    //Set Timeout
    wait_interval = SEC_BASED * multiplier;
    wait_interval = ((MAX_INTERVAL)<(wait_interval))? MAX_INTERVAL:wait_interval;
    multiplier = multiplier*POWER_BASED;
    timeout.tv_sec = wait_interval;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout,sizeof(timeout));

    send_respond = sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&serverAddr, addr_size);

    // Fail to send message
    if (send_respond!=0){
        printf("Number of attampt: %d ; Waiting interval: %d\n",i,wait_interval);
    }else{
        // Success 
        // Receive echo message from server 
        bzero(buffer, 1024);
        recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&serverAddr, &addr_size);
        printf("[Message received] %s \n",buffer);
        exit(0);

    }
  }
  printf("Faild to send message to server\n");
  exit(1);
  
}
