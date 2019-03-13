#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

#include <pthread.h>


void* receive(void *sock) {
    char buffer[1024] = {0};
    int valread;
    if (valread = read( (int)sock , buffer, 1024) > 0) {
        if (!strcmp((char*)buffer, "\n")) {
            printf("chat ended by client\n");
            exit(0);
            //return NULL;
        } else {
          printf("client: %s\n", (char*)buffer );
            return NULL;
        }
    }
    exit(1);
    //return NULL;
}

void* send_(void *sock) {
    char* buf = NULL;
    size_t n = 0;
    if (getline(&buf, &n, stdin) > 0) {
        if (!strcmp(buf, "\n")) {
            printf("chat ended by server.\n");
            exit(0);
            //return NULL;
        } else {
            send((int)sock , buf , strlen(buf) , 0 );
            printf("server: %s\n",buf );
            return NULL;
        }
    }
    exit(1);
    //return NULL;
}

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 

    pthread_t id1, id2;
    while(1) {
        pthread_create(&id1, NULL, receive, (void*) new_socket);
        pthread_create(&id2, NULL, send_, (void*) new_socket);
    }


//    char* buf = NULL;
//    size_t n = 0;
/*
    while (getline(&buf, &n, stdin) > 1) {
        if (!strcmp(buf, "\n")) {
            break;
        }
        send(new_socket , buf , strlen(buf) , 0 );
        //printf("Hello message sent\n");
        valread = read( new_socket , buffer, 1024);
        printf("client: %s\n", buffer);
        //printf("server: %s\n",buffer );
    }

    valread = read( new_socket , buffer, 1024); 
    printf("%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); */
    return 0; 
} 
