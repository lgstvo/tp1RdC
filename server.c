#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

int socketIpV4(char const *port_str)
{
    int domain, address_v4_size, serverfd, new_fd, opt = 1;
    struct sockaddr *address;
    struct sockaddr_in address_v4;
    int port = atoi(port_str);

    address_v4.sin_family = AF_INET;
    address_v4.sin_port = htons(port);
    address_v4.sin_addr.s_addr = htonl(INADDR_ANY);

    domain = AF_INET;
    address_v4_size = sizeof(address_v4);
    address = (struct sockaddr *)&address_v4;

    if ((serverfd = socket(domain, SOCK_STREAM, 0)) == 0) {
        printf("create socket error.\n");
        exit(-1);
    }

    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        printf("socket option error.\n");
        exit(-1);
    }

    if (bind(serverfd, address, address_v4_size) < 0) {
        printf("bind error.\n");
        exit(-1);
    }

    if (listen(serverfd, 5) < 0) {
        printf("listen error.\n");
        exit(-1);
    }

    int address_size = sizeof(address);
    if ((new_fd = accept(serverfd, (struct sockaddr *)&address, (socklen_t *)&address_size)) < 0) {
        printf("accept connection error.\n");
        exit(-1);
    }
    
    return new_fd;
}

int socketIpV6(char const *port_str) {
    int domain, address_v4_size, serverfd, new_fd, opt = 1;
    struct sockaddr *address;
    struct sockaddr_in6 address_v6;
    int port = atoi(port_str);

    address_v6.sin6_family = AF_INET6;
    address_v6.sin6_port = htons(port);
    address_v6.sin6_addr = in6addr_any;

    domain = AF_INET6;
    address_v4_size = sizeof(address_v6);
    address = (struct sockaddr *)&address_v6;


    if ((serverfd = socket(domain, SOCK_STREAM, 0)) == 0) {
        printf("create socket error.\n");
        exit(-1);
    }

    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        printf("socket option error.\n");
        exit(-1);
    }

    if (bind(serverfd, address, address_v4_size) < 0) {
        printf("bind error.\n");
        exit(-1);
    }

    if (listen(serverfd, 5) < 0) {
        printf("listen error.\n");
        exit(-1);
    }

    int address_size = sizeof(address);
    if ((new_fd = accept(serverfd, (struct sockaddr *)&address, (socklen_t *)&address_size)) < 0) {
        printf("accept connection error.\n");
        exit(-1);
    }
    return new_fd;
}

void validade_comm(int data)
{
    if (data < 0) exit(-1);
}



int main(int argc, char const *argv[])
{
    if (argc != 3) {
        printf("Expected 3 inputs.\n");
        exit(-1);
    }

    int sock = strcmp(argv[1], "v6") == 0 ? socketIpV6(argv[2]) : socketIpV4(argv[2]);

    char buffer[500];  
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        int data = read(sock, buffer, sizeof(buffer));
        validade_comm(data);
        printf("%s\n", buffer);
        data = write(socK, buffer, strlen(buffer));
        validade_comm(data);
    }
}