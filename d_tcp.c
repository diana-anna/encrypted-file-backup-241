#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CAPACITY 256

extern int errno;

int server(char* ip_addr, char* file_name){

  int bytesReceived = 0;
  int clientSocket;
  char recvBuff[CAPACITY];
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
  serverAddr.sin_addr.s_addr = inet_addr(ip_addr);
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
  fp = fopen(file_name, "ab"); 
  if(fp == NULL){
      perror("Error opening file");
      exit(1);
  }

  // Receive data in chunks of 256 bytes 
  while((bytesReceived = read(clientSocket, recvBuff, CAPACITY)) > 0)
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

int server(){
  int welcomeSocket, newSocket;
  char buffer[1024];
  char* serverIP;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  struct ifaddrs *addrs = malloc(sizeof(struct ifaddrs));
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

  //get this machine's IP address
  getifaddrs(&addrs);
  struct ifaddrs *tmp = addrs;

  while (tmp) 
  {
      if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
      {
          struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
          //printf("%s: %s\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr));
          if (strcmp(tmp->ifa_name, "eth0") == 0){
            serverIP = inet_ntoa(pAddr->sin_addr);
            printf("The server's IP address is %s\n", serverIP);
          }
      }

      tmp = tmp->ifa_next;
  }

  freeifaddrs(addrs);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(7891);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr(serverIP);
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  //*---- Listen on the socket, with 5 max connection requests queued ----/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  newSocket = accept(welcomeSocket, (struct sockaddr*)NULL ,NULL);

  /* Open the file that we wish to transfer */
  FILE *fp = fopen("sample_file.txt","rb");
  if(fp==NULL)
  {
      perror("File open error");
      exit(1);   
  }   

  /* Read data from file and send it */
  while(1)
  {
      /* First read file in chunks of 256 bytes */
      unsigned char buff[256]={0};
      int nread = fread(buff,1,256,fp);
      printf("Bytes read %d \n", nread);        

      /* If read was success, send data. */
      if(nread > 0)
      {
          printf("Sending \n");
          write(newSocket, buff, nread);
      }

      /*
        * There is something tricky going on with read .. 
        * Either there was error, or we reached end of file.
        */
      if (nread < 256)
      {
          if (feof(fp))
              printf("End of file\n");
          if (ferror(fp))
              printf("Error reading\n");
          break;
      }


  }

  close(newSocket);

  return 0;
}

int main(int argc, char **argv){
    char* usage_msg = "Usage: must specify server or client\n";

    if (argc == 1){
        printf(usage_msg);
        exit(1);
    }

    char* mode = argv[1];
    if (strcmp(mode, "client") == 0){
        client();
    } else if (strcmp(mode, "server") == 0){
        if (argc != 3){
            printf("Usage: program server filename")
        }
        server();
    } else{
        printf(usage_msg);
        exit(1);
    }

    return 0;
}