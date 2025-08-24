#include <asm-generic/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <fcntl.h>

#include "tcp.h"

int TCP::listenNet(std::string& ip, std::string& port) {
    int optval = 1;

    /* Create new connection and add to linked list */
    struct serverSocket* connect = new serverSocket;

    /* Create server socket */
    if ((connect->serverSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Failed to create socket");
    
    /* Check for the existence of such a socket in the system for reusing it with same addr */
    if (setsockopt(connect->serverSockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
        throw std::runtime_error("Failed to set socket options");

    /* Check for the existence of such a socket in the system for reusing it with same port */
    if (setsockopt(connect->serverSockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int)) < 0)
        throw std::runtime_error("Failed to set socket options");

    /* Set nonblocking for server fd */
    int flags = fcntl(connect->serverSockfd, F_GETFL, 0);
    fcntl(connect->serverSockfd, F_SETFL, flags | O_NONBLOCK);
    
    /* Fill local address structure */
    memset(&connect->serverAddress, 0, sizeof(connect->serverAddress));
    connect->serverAddress.sin_family = AF_INET;
    connect->serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    connect->serverAddress.sin_port = htons(atoi(port.c_str()));

    /* Bind to the local address and port */
    if (bind(connect->serverSockfd, (struct sockaddr *)&connect->serverAddress,
            sizeof(connect->serverAddress)) < 0)
        throw std::runtime_error("Failed to bind to socket");
    
    /* Mark the socket so it will listen for incoming connections */
    if (listen(connect->serverSockfd, SOMAXCONN) < 0)
        throw std::runtime_error("Failed to listen on port");
    
    return connect->serverSockfd;
}

int TCP::acceptNet(int listenServerSocket) {
    return accept(listenServerSocket, NULL, NULL);
}

int TCP::closeNet(int conn) {
    return close(conn);
}

int TCP::sendNet(int conn, const std::string& buffer) {
    return send(conn, buffer.c_str(), buffer.length(), 0);
}

int TCP::recvNet(int conn, char* buffer, size_t size) {
    int num = recv(conn, buffer, (int)size, 0);
    return num;
}