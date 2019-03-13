#include <unistd.h>
#include <arpa/inet.h>
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
            printf("chat ended by server\n");
            exit(0);
            //return NULL;
        } else {
          printf("server: %s\n", (char*)buffer );
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
            printf("chat ended by client.\n");
            exit(0);
            //return NULL;
        } else {
            send((int)sock , buf , strlen(buf) , 0 );
            printf("client: %s\n",buf );
            return NULL;
        }
    }
    exit(1);
    //return NULL;
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    pthread_t id1, id2;
    while(1) {
        pthread_create(&id1, NULL, receive, (void*)sock);
        pthread_create(&id2, NULL, send_, (void*)sock);
    }
    return 0;
}

