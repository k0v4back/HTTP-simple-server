#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include "tcp.h"

#define METHOD_SIZE 16
#define PATH_SIZE 2048
#define PROTO_SIZE 16

#define BUF_SIZE 1024

#define _READ_METHOD    0
#define _READ_PATH      1
#define _READ_PROTO     2

enum responseType {
    RESPONSE200 = 0,
    RESPONSE404 = 1
};

class HTTP {
private:
    std::string hostAddr;
    std::string hostPort;
    std::unordered_map<std::string, std::string> ht;
public:
    HTTP(std::string addr, std::string ip);

    std::string getHostAddr() const;
    std::string getHostPort() const;
    std::unordered_map<std::string, std::string> const& getHT() const;

    void handleHttp(std::string addr, std::string file);                    /* Fill hash table */
    void displayPage(int conn, std::string& file);                          /* Display HTML page requested by user */
    int listenHttp(void);                                                   /* Listen client requests */
    void HTTPResponse(int conn, std::string& fileName, responseType rt);    /* Parse HTTP request for HTML page */
    int switchHttp(int conn, std::string& path);                            /* Analyze http request */
    void page404Http(int conn);                                             /* Display 404 */

    TCP tcp;

    class HTTPreq {
    public:
        std::string method;
        std::string path;
        std::string proto;

        void parseRequest(std::string& buffer, size_t size);    /* Parse HTTP request from client */
    };
};
