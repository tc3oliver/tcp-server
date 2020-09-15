#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>


#define PORT 8111
#define MESSAGE_SIZE 1024

int main() {

    int ret = -1;
    int socket_fd, accept_fd;
    int flag = 1;
    int backlog = 10;

    struct sockaddr_in local_addr{}, remote_addr{};

    char in_buff[MESSAGE_SIZE] = {0,};

    //create a tcp socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        std::cout << "Failed to create a socket!" << std::endl;
        exit(-1);
    }

    //set option of socket
    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    if (ret == -1) {
        std::cout << "Failed to set socket options!" << std::endl;
        exit(-1);
    }

    //set local address
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = PORT;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(local_addr.sin_zero), 8);

    //bind socket
    ret = bind(socket_fd, (struct sockaddr *) &local_addr, sizeof(struct sockaddr_in));
    if (ret == -1) {
        std::cout << "Failed to bind addr!" << std::endl;
        exit(-1);
    }

    // listen
    ret = listen(socket_fd, backlog);
    if (ret == -1) {
        std::cout << "Failed to listen socket!" << std::endl;
        exit(-1);
    }

    // loop
    for (;;) {
        socklen_t addr_len = sizeof(struct sockaddr);
        //accept an new connection
        accept_fd = accept(socket_fd, (struct sockaddr *) &remote_addr, &addr_len);
        for (;;) {
            //receive network data and print it
            ret = recv(accept_fd, (void *) in_buff, MESSAGE_SIZE, 0);
            if (ret == 0) {
                break;
            }
            std::cout << "receive: " << in_buff << std::endl;
            send(accept_fd, (void *) in_buff, MESSAGE_SIZE, 0);
        }
        std::cout << "close client connection..." << std::endl;
        close(accept_fd);
    }
    std::cout << "quit server..." << std::endl;
    close(socket_fd);

    return 0;
}
