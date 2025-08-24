#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <list>

class TCP {
public:
    TCP() = default;
    ~TCP() = default;

    class serverSocket {
    public:
        int serverSockfd;
        struct sockaddr_in serverAddress;
        bool acceptedSuccessfully;
        int error;
    };

    int listenNet(std::string& ip, std::string& port);  /* Start server */
    int acceptNet(int listenServerSocket);              /* Accept connections by server */
    int closeNet(int conn);                             /* Server or client close connection */

    int sendNet(int conn, const std::string& buffer);   /* Send request */
    int recvNet(int conn, char* buffer, size_t size);   /* Receive */
};
