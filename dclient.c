/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

extern int errno;

int main(){
  int bytesReceived = 0;
  int clientSocket;
  char recvBuff[256];
  memset(recvBuff, '0', sizeof(recvBuff));
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(7891);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("172.22.146.60");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  if((connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)) == -1){
      perror("Connection Failed!");
      exit(1);
  }

  // Create file where data will be stored 
  FILE *fp;
  fp = fopen("client_sample.txt", "ab"); 
  if(NULL == fp)
  {
      printf("Error opening file");
      return 1;
  }

  // Receive data in chunks of 256 bytes 
  while((bytesReceived = read(clientSocket, recvBuff, 256)) > 0)
  {
      printf("Bytes received %d\n",bytesReceived);    
      fwrite(recvBuff, 1,bytesReceived,fp);
  }

  if(bytesReceived < 0)
  {
      fprintf(stderr, " Read Error: errno %d \n", errno);
      perror("read error\n");
  }


  return 0;
}