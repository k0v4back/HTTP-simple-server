#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "tcp.h"
#include "../libs/Thread-pool/src/thread_pool.h"

#define METHOD_SIZE 16
#define PATH_SIZE 2048
#define PROTO_SIZE 16

#define BUF_SIZE 1024 * 1024

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
    class HTTPresp {
    public:
        std::string method;
        std::string path;
        std::string proto;

        void parseRequest(std::string& buffer, size_t size);    /* Parse HTTP request from client */
    };

    HTTP(std::string addr, std::string ip);

    std::string getHostAddr() const;
    std::string getHostPort() const;
    std::unordered_map<std::string, std::string> const& getHT() const;

    void handleHttp(std::string addr, std::string file);                                    /* Fill hash table */
    void displayPage(int conn, std::string& file, HTTPresp* req);                           /* Display HTML page requested by user */
    int listenHttp(tp::ThreadPoll& threadPool);                                             /* Listen client requests */
    void HTTPResponse(int conn, std::string& fileName, responseType rt, HTTPresp* req);     /* Parse HTTP request for HTML page */
    int switchHttp(int conn, HTTPresp* req);                                                /* Analyze http request */
    void page404Http(int conn, HTTPresp* req);                                              /* Display 404 */

    TCP tcp;
};
