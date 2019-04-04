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

int check_for_file(char* file_name){
    //chdir("../drive");
    FILE *fp = fopen(file_name, "r");
    if (fp) {
        return 1;
    } else {
        return 0;
    }
}

int send_file(int Socket, char* file_name){
    /* Open the file that we wish to transfer */
    FILE *fp = fopen(file_name,"rb");
    if(fp==NULL)
    {
        perror("File open error");
        exit(1);   
    }   

    /* Read data from file and send it */
    while(1)
    {
        /* First read file in chunks of 256 bytes */
        unsigned char buff[CAPACITY]={0};
        int nread = fread(buff,1,CAPACITY,fp);
        printf("Bytes read %d \n", nread);        

        /* If read was success, send data. */
        if(nread > 0)
        {
            printf("Sending \n");
            write(Socket, buff, nread);
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
    return 0;
}

int receive_file(int Socket, char* file_name){
    int bytesReceived = 0;
    char recvBuff[CAPACITY];
    char cmdBuff[CAPACITY];
    memset(recvBuff, 0, CAPACITY);
    memset(cmdBuff, 0, CAPACITY);

    //check if file exists
    sprintf(cmdBuff, "doesExist ");
    strcat(cmdBuff, file_name);
    send(Socket, cmdBuff, CAPACITY, 0);
    recv(Socket, cmdBuff, CAPACITY, 0);
    int exists_file = atoi(cmdBuff);

    if (exists_file){

        // Create file where data will be stored 
        FILE *fp;
        fp = fopen(file_name, "ab"); 
        if(fp == NULL){
            perror("Error opening file");
            exit(1);
        }

        // Receive data in chunks of CAPACITY bytes 
        while((bytesReceived = read(Socket, recvBuff, CAPACITY)) > 0){
            printf("Bytes received %d\n",bytesReceived);    
            fwrite(recvBuff, 1, bytesReceived, fp);
        }

        if(bytesReceived < 0){
            perror("read error");
            exit(1);
        }
    } else{
        printf("File does not exist!\n");
        exit(1);
    }


    return 0;
}

int client(char* ip_addr){

    int bytesReceived = 0;
    int clientSocket;
    char recvBuff[CAPACITY];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char* input = NULL;
    char* mode;
    char* file_name;

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

    printf("Please enter a command.\n");
    size_t line_size;
    if (getline(&input, &line_size, stdin) == -1){
        perror("getline failed");
        exit(1);
    }
    mode = strtok(input, " ");
    file_name = strtok(NULL, " ");
    if (strcmp(mode, "receive") == 0){
        receive_file(clientSocket, file_name);
    }


    /*
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
        //fprintf(stderr, " Read Error: errno %d \n", errno);
        perror("read error\n");
    }
    */


    return 0;
}

int server(){
    int welcomeSocket, newSocket;
    //char buffer[1024];
    char* serverIP = NULL;
    struct sockaddr_in serverAddr;
    //struct sockaddr_storage serverStorage;
    struct ifaddrs *addrs = malloc(sizeof(struct ifaddrs));
    //socklen_t addr_size;
    char action[1024];

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
            printf("%s: %s\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr));
        }

        tmp = tmp->ifa_next;
    }

    freeifaddrs(addrs);

    //Let the user decide which IP address to use as the server's IP address
    size_t line_size;
    printf("Please input the IP address you would like to use for this server: \n");
    if (getline(&serverIP, &line_size, stdin) == -1){
        perror("getline failed");
        exit(1);
    }

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

    if (recv(newSocket, action, 1024, 0) == -1){
        perror("recv failed");
        exit(1);
    }
    char* mode = strtok(action, " ");
    char* file_name = strtok(NULL, " ");

    if (strcmp(mode, "doesExist") == 0){
        //we want to check if a file exists on the server
        if (file_name == NULL){
            printf("No filename found!\n");
            exit(1);
        }
        int file_exists = check_for_file(file_name);

        if (!file_exists){
            action[0] = '0';
            action[1] = 0;
            send(newSocket, action, CAPACITY, 0);
            printf("File doesn't exist!\n");
            exit(1);
        } else{
            action[0] = '1';
            action[1] = 0;
            send(newSocket, action, CAPACITY, 0);
            send_file(newSocket, file_name);
        }
    }

    close(newSocket);

  return 0;
}

int main(int argc, char **argv){
    char* usage_msg = "Usage: must specify server or client\n";

    if (argc == 1){
        printf("%s", usage_msg);
        exit(1);
    }

    char* mode = argv[1];
    if (strcmp(mode, "server") == 0){
        server();
    } else if (strcmp(mode, "client") == 0){
        if (argc != 3){
            printf("Usage: program client IP_addrs\n");
            exit(1);
        }
        client(argv[2]);
    } else{
        printf("%s", usage_msg);
        exit(1);
    }

    return 0;
}