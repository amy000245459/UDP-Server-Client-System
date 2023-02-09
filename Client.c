#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
// Define fixed variable for waiting interval between retries
const int POWER_BASED = 2;
const float SEC_BASED = 0.5; // 500ms
const int MAX_INTERVAL = 8; //8000ms = 8s 
 
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
  int sockfd, respond,i; 
  float wait_interval; 
  int try_num = 1;
  int multiplier = POWER_BASED;
  struct sockaddr_in serverAddr;
  struct timeval timeout = {.tv_sec=1,.tv_usec=0};
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
  
  //Set timeout for socket
  setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout,sizeof(timeout));
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout,sizeof(timeout));
  
  // Send message to server with retry option
  addr_size = sizeof(serverAddr);   
  printf("[Message sent] %s \n",buffer);
  while(try_num <= max_try){

    sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&serverAddr, addr_size);
    
    // Receive echo message from server 
    bzero(buffer, 1024);
    respond = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&serverAddr, &addr_size);
    
    if (respond!=-1){
        // Success 
        printf("[Message received] %s \n",buffer);
        exit(0);
        }
        
    // setup waitting_time 
    wait_interval = SEC_BASED * multiplier;
    wait_interval = ((MAX_INTERVAL)<(wait_interval))? MAX_INTERVAL:wait_interval;
    printf("Number of attampt: %d ; Waiting interval: %f(s) \n",try_num,wait_interval);

    sleep((int)wait_interval);
    try_num ++;
    multiplier = multiplier*POWER_BASED;
  }
  printf("Faild to send message to server\n");
  exit(1);
  
}
