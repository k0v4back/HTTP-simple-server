#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <vector>

#include "tcp.h"

int TCP::listenNet(std::string& ip, std::string& port) {
    int optval = 1;

    /* Create new connection and add to linked list */
    struct acceptedSocket* connect = new acceptedSocket;
    listAcceptedSocket.push_back(*connect);

    /* Create server socket */
    if ((connect->clientSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    
    /* Check for the existence of such a socket in the system for reusing it */
    if (setsockopt(connect->clientSockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
        return -1;
    
    /* Fill local address structure */
    memset(&connect->clientAddress, 0, sizeof(connect->clientAddress));
    connect->clientAddress.sin_family = AF_INET;
    connect->clientAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    connect->clientAddress.sin_port = htons(atoi(port.c_str()));

    /* Bind to the local address */
    if (bind(connect->clientSockfd, (struct sockaddr *)&connect->clientAddress,
            sizeof(connect->clientAddress)) < 0)
        return -1;
    
    /* Mark the socket so it will listen for incoming connections */
    if (listen(connect->clientSockfd, SOMAXCONN) < 0)
        return -1;
    
    return connect->clientSockfd;
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

int TCP::recvNet(int conn, std::string& buffer, size_t size) {
    char* buf_c = (char*)buffer.data();
    int num = recv(conn, buf_c, (int)size, 0);
    return num;
}