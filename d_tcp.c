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
#include <stdint.h>
#include "aes.h"

#define CAPACITY 256

int check_for_file(char* file_name){
    printf("Checking for file %s\n", file_name);
    FILE *fp = fopen(file_name, "r");
    if (fp) {
        return 1;
    } else {
        return 0;
    }
}

int send_file(int Socket, char* file_name){

    char* msgBuff = malloc(CAPACITY);
    memset(msgBuff, 0, CAPACITY);

    if (file_name == NULL){
        printf("No filename found!\n");
        exit(1);
    }

    //check if the file exists
    int file_exists = check_for_file(file_name);
    if (!file_exists){
        msgBuff[0] = '0';
        msgBuff[1] = 0;
        send(Socket, msgBuff, CAPACITY, 0);
        printf("File doesn't exist!\n");
        exit(1);
    } else{
        msgBuff[0] = '1';
        msgBuff[1] = 0;
        send(Socket, msgBuff, CAPACITY, 0);
    }

    //open the file
    FILE *fp = fopen(file_name,"rb");
    if(fp==NULL)
    {
        perror("File open error");
        exit(1);   
    }
    //read in file size
    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    //send file size across the socket
    const int necessary_buf_size = snprintf(NULL, 0, "%ld", file_size);
    if (necessary_buf_size >= CAPACITY){
        msgBuff = realloc(msgBuff, necessary_buf_size + 1);
    }
    int used_buf_bytes = snprintf(msgBuff, necessary_buf_size + 1, "%ld", file_size);
    if (used_buf_bytes != necessary_buf_size){
        perror("Something went wrong with sending the size of the file!");
        exit(1);
    }
    write(Socket, msgBuff, used_buf_bytes+1);
    printf("%s size: %ld bytes\n", file_name, file_size);


    // Read data from file and send it
    long byte_count = 0;
    while(1)
    {
        // First read file in chunks of CAPACITY bytes
        unsigned char buff[CAPACITY]={0};
        int nread = fread(buff,1,CAPACITY,fp);    
        
        // ENCRYPTION
        // taken from example (credited in aes.h, aes.c, aes_test.c)
        WORD key_schedule[60];
        BYTE enc_buf[256]; // holds encrypted buff
        BYTE iv[1][16] = {
        {0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff},
       };
        BYTE key[1][32] = {
        {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4}
       };
       int pass = 1;

       aes_key_setup(key[0], key_schedule, 256);
       aes_encrypt_ctr(buff, CAPACITY, enc_buf, key_schedule, 256, iv[0]);

        // If read was success, send data.
        if(nread > 0){
            //printf("Sending \n");
            ssize_t bytes_written = write(Socket, buff, nread);
            byte_count += bytes_written;
        }

        // There is something tricky going on with read, either there was error, or we reached end of file.
        if (feof(fp)){
            printf("End of file\n");
            break;
        }
        if (ferror(fp)){
            printf("Error reading\n");
            break;
        }


    }
    fclose(fp);
    return 0;
}

int receive_file(int Socket, char* file_name){
    int bytesReceived = 0;
    char recvBuff[CAPACITY];
    char msgBuff[CAPACITY];
    memset(recvBuff, 0, CAPACITY);
    memset(msgBuff, 0, CAPACITY);

    //wait for response -- does file exist?
    recv(Socket, msgBuff, CAPACITY, 0);
    int exists_file = atoi(msgBuff);

    if (exists_file){
        // Create file where data will be stored 
        FILE *fp;
        fp = fopen(file_name, "ab"); 
        if(fp == NULL){
            perror("Error opening file");
            exit(1);
        }
        //receive size of file
        recv(Socket, msgBuff, CAPACITY, 0);
        long file_size = atoi(msgBuff);
        long byte_count = 0;
        printf("%s size: %ld bytes\n", file_name, file_size);

        // Receive data in chunks of CAPACITY bytes 
        while((bytesReceived = read(Socket, recvBuff, CAPACITY)) >= 0){
            
            // DECRYPTION
            WORD key_schedule[60];
            BYTE dec_buf[256]; // holds decrypted buff
            BYTE iv[1][16] = {
            {0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff},
           };
            BYTE key[1][32] = {
            {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4}
           };
           int pass = 1;

           aes_key_setup(key[0], key_schedule, 256);
           aes_decrypt_ctr(recvBuff, CAPACITY, dec_buf, key_schedule, 256, iv[0]);
            
            if (bytesReceived < CAPACITY){
                printf("Bytes received %d\n",bytesReceived);
            }
            byte_count += bytesReceived;  
            fwrite(recvBuff, 1, bytesReceived, fp);
            if (byte_count == file_size){
                printf("End of file! Total bytes received: %ld bytes\n", byte_count);
                fclose(fp);
                break;
            }
            else if(byte_count > file_size){
                perror("Tried to write more bytes than the original file!");
                exit(1);
            }
        }

        if(bytesReceived < 0){
            perror("read error");
            exit(1);
        }
    } 
    else{
        printf("File does not exist!\n");
        exit(1);
    }



    return 0;
}

int client(char* ip_addr, char* drive_dir){
    int bytesReceived = 0;
    int clientSocket;
    char recvBuff[CAPACITY];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char* input = NULL;
    size_t line_size;
    char* mode;
    char* file_name;

    if (drive_dir != NULL){
        chdir(drive_dir);
    }

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr(ip_addr);
    // Set all bits of the padding field to 0
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    addr_size = sizeof serverAddr;
    if((connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)) == -1){
        perror("Connection Failed!");
        exit(1);
    }

    while(1){

        printf("Please enter a command.\n");
        if (getline(&input, &line_size, stdin) == -1){
            perror("getline failed");
            exit(1);
        }
        input[strlen(input)-1] = 0; //get rid of new line char
        send(clientSocket, input, CAPACITY, 0);
        mode = strtok(input, " ");
        file_name = strtok(NULL, " ");
        if (strcmp(mode, "receive") == 0){
            receive_file(clientSocket, file_name);
        }
        else if (strcmp(mode, "send") == 0){
            send_file(clientSocket, file_name);
        } 
        else if (strcmp(mode, "close") == 0){
            break;
        }
    }

    close(clientSocket);

    return 0;
}

int server(char* drive_dir){
    int welcomeSocket, newSocket;
    char* serverIP = NULL;
    struct sockaddr_in serverAddr;
    struct ifaddrs *addrs = malloc(sizeof(struct ifaddrs));
    char action[1024];

    if (drive_dir != NULL){
        chdir(drive_dir);
    }

    // Create the socket. The three arguments are:
    // 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case)
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

    // Configure settings of the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    // Set all bits of the padding field to 0
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    // Listen on the socket, with 5 max connection requests queued
    if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
    else
    printf("Error\n");



    newSocket = accept(welcomeSocket, (struct sockaddr*)NULL ,NULL);

    while(1){

        if (recv(newSocket, action, 1024, 0) == -1){
            perror("recv failed");
            exit(1);
        }
        char* mode = strtok(action, " ");
        char* file_name = strtok(NULL, " ");
        printf("Command entered: %s %s\n", mode, file_name);
        if (strcmp(mode, "receive") == 0){
            //we need to send file to client
            send_file(newSocket, file_name);
        }
        else if (strcmp(mode, "send") == 0){
            //we need to receive the client's file
            receive_file(newSocket, file_name);
        }
        else if (strcmp(mode, "close") == 0){
            break;
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
        if (argc == 2){
            server(NULL);
        } else if (argc == 3){
            server(argv[2]);
        }
        else{
            printf("Usage: program server optional_path_to_drive\n");
        }
    } 
    else if (strcmp(mode, "client") == 0){
        if (argc < 3){
            printf("Usage: program client IP_addrs optional_path_to_drive\n");
            exit(1);
        } else if (argc == 3){
            client(argv[2], NULL);
        } else if (argc == 4){
            client(argv[2], argv[3]);
        } else{
            printf("Usage: program client IP_addrs ");
        }
    } else{
        printf("%s", usage_msg);
        exit(1);
    }

    return 0;
}
