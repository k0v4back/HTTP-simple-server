#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "tcp.h"
#include "../libs/Thread-pool/include/thread_pool.h"
#include "http_parser/http_parser.h"

#define METHOD_SIZE 16
#define PATH_SIZE 2048
#define PROTO_SIZE 16

#define BUF_SIZE 1024 * 1024

#define _READ_METHOD    0
#define _READ_PATH      1
#define _READ_PROTO     2

enum responseType {
    RESPONSE_200,
    RESPONSE_404
};

enum contentType {
    CONTENT_GIF,
    CONTENT_JPEG,
    CONTENT_PNG,
    CONTENT_ICON,
    CONTENT_CSV,
    CONTENT_HTML,
    CONTENT_XML
};

class HTTP {
private:
    std::string hostAddr;
    std::string hostPort;

    /* URI, method, file name */
    std::map<std::string, std::map<HTTPMethod, std::string>> ht;

public:
    class HTTPresp {
    public:
        std::string method;
        std::string path;
        std::string proto;

        responseType rt;
        contentType ct;

        void parseRequest(std::string& buffer, size_t size);    /* Parse HTTP request from client */
    };

    HTTP(std::string addr, std::string ip);

    std::string getHostAddr() const;
    std::string getHostPort() const;
    std::map<std::string, std::map<HTTPMethod, std::string>> const& getHT() const;

    void handleHttp(std::string addr, HTTPMethod method, std::string file); /* Fill hash table */
    int listenHttp(tp::ThreadPoll& threadPool);                             /* Listen client requests */
    void HTTPResponse(int conn, std::string& fileName, HTTPresp& req);      /* Parse HTTP request for HTML page */
    int switchHttp(int conn, HTTPresp& req);                                /* Analyze http request */
    void page404Http(int conn, HTTPresp& req);                              /* Display 404 */

    TCP tcp;    /* Transport level */

private:
    HTTPresp resp;
};
