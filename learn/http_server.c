#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8888
#define BUFFER_SIZE 4096
#define RESPONSE_HEADER "HTTP/1.1 200 OK\r\nConnection: close\r\nAccpet-Ranges:bytes\r\nContent-Type:text/html\r\n\r\n"
#define RESPONSE_BODY "<h1>Hello!</h1>"

int handle(int conn)
{
    int len = 0;
    char buffer[BUFFER_SIZE];
    char *pos = buffer;
    
    bzero(buffer,BUFFER_SIZE);
    len = recv(conn, buffer, BUFFER_SIZE, 0);
    if(len <= 0)
    {
        printf("Recv Error\n");
        return -1;
    }
    else
    {
        printf("Debug request:\n------------\n%s",buffer);
    }
    send(conn, RESPONSE_HEADER, sizeof(RESPONSE_HEADER), 0);
    send(conn, RESPONSE_BODY, sizeof(RESPONSE_BODY), 0);
    close(conn);
}

int main(int argc, char *argv[])
{
    int port = PORT;
    struct sockaddr_in client_sockaddr;
    struct sockaddr_in server_sockaddr;
    
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    int conn;
    socklen_t length = sizeof(struct sockaddr_in);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listenfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr)))
    {
        printf("Bind Error!\n");
        return -1;
    }
    if(listen(listenfd, 10) < 0 )
    {
        printf("Listen Error!\n");
        return -1;
    }

    while(1)
    {
        conn = accept(listenfd,(struct sockaddr*)&client_sockaddr, &length);
        if(conn < 0)
        {
            printf("Connect Error!\n");
            continue;
        }
        if(handle(conn) < 0)
        {
            printf("Handle Error!\n");
            continue;
        }
    }
    return 0;
}