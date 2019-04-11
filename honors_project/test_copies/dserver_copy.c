/****************** SERVER CODE ****************/

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


int main(){
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
