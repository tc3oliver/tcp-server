//
// Created by oliver on 2020/9/15.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#define SERVER_PORT 8111
#define MESSAGE_SIZE 1024

int main() {

    int socket_fd;
    int ret;
    struct sockaddr_in serverAddr{};

    char sendbuf[MESSAGE_SIZE] = {0,};
    char recvbuf[MESSAGE_SIZE] = {0,};
    // create
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        std::cout << "Failed to create a socket!" << std::endl;
        exit(-1);
    }


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = SERVER_PORT;
    serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    // connect
    ret = connect(socket_fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (ret < 0 ) {
        std::cout << "Failed to connect server!" << std::endl;
        exit(-1);
    }

    while (true) {
        memset(sendbuf, 0, MESSAGE_SIZE);
        fgets(sendbuf, MESSAGE_SIZE, stdin);
        ret = send(socket_fd, sendbuf, strlen(sendbuf), 0);
        if (ret <= 0) {
            std::cout << "Failed to send data!" << std::endl;
            break;
        }

        if (strcmp(sendbuf, "quit") == 0) {
            break;
        }

        ret = recv(socket_fd, recvbuf, MESSAGE_SIZE, 0);
        recvbuf[ret] = '\0';
        std::cout << "recv: " << recvbuf << std::endl;
    }
    close(socket_fd);
}