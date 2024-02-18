#ifndef _HTTP_H_
#define _HTTP_H_

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

#define THREAD_NUM 8

enum responseType {
    RESPONSE200 = 0,
    RESPONSE404 = 1
};

struct threadArgs {
    int listenFd;
    int epollFd;
    class TCP* tcp;
    class HTTP* http;
};

class HTTP {
public:
    HTTP();
    HTTP(std::string addr, std::string ip);

    void handleHttp(std::string addr, std::string file);                    /* Fill hash table */
    void displayPage(int conn, std::string& file);                          /* Display HTML page requested by user */
    int listenHttp(void);                                                   /* Listen client requests */
    void parseHtmlHttp(int conn, std::string& fileName, responseType rt);   /* Parse HTTP request for HTML page */
    int switchHttp(int conn, std::string& path);                            /* Analyze http request */
    void page404Http(int conn);                                             /* Display 404 */
    static void* serverThread(void* args);                                         /* Server thread */

    std::string hostAddr;
    std::string hostIp;
    std::unordered_map<std::string, std::string> ht;
    TCP tcp;

    class HTTPreq {
    public:
        std::string method;
        std::string path;
        std::string proto;
        uint8_t state = 0;
        size_t index = 0;

        void parseRequest(std::string& buffer, size_t size);    /* Parse HTTP request from client */
    };
};

#endif /* _HTTP_H_ */
