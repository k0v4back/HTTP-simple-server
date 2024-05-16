#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <list>

class TCP {
public:
    TCP() = default;
    ~TCP() = default;

    class acceptedSocket {
    public:
        int clientSockfd;
        struct sockaddr_in clientAddress;
        bool acceptedSuccessfully;
        int error;
    };

    int listenNet(std::string& ip, std::string& port);  /* Start server */
    int acceptNet(int listenServerSocket);              /* Accept connections by server */
    int closeNet(int conn);                             /* Server or client close connection */

    int sendNet(int conn, const std::string& buffer);           /* Send request */
    int recvNet(int conn, std::string& buffer, size_t size);    /* Recive */
};
