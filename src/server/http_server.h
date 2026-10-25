#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <mutex>

#include "thread_pool.h"

#include "http/http_response.h"
#include "http/http_parser.h"
#include "net/tcp.h"
#include "config/config_parser.h"

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

private:
    class Connection {
    public:
        std::string request_buffer; /* Request data from user */
        bool request_processing = false; 
    };

    std::unordered_map<int, Connection> connections; /* Store all connections here: socket fd -> connection */

public:
    class HTTPresp {
    public:
        std::string method;
        std::string path;
        std::string proto;

        responseType rt;
        contentType ct;

        void parseRequest(std::string buffer, size_t size);    /* Parse HTTP request from client */
    };

    HTTP(net::tcp_server_info tcp_server_info) : tcp_server(tcp_server_info) {}

    std::string getHostAddr() const;
    std::string getHostPort() const;
    std::map<std::string, std::map<HTTPMethod, std::string>> const& getHT() const;

    void handleHttp(std::string addr, HTTPMethod method, std::string file);                 /* Fill hash table */
    int listenHttp(tp::ThreadPoll& threadPool, const config::config_parser& config);        /* Listen client requests */
    void HTTPResponse(net::fd_t fd_t, const std::string& fileName, HTTPresp& req);                /* Parse HTTP request for HTML page */
    int switchHttp(net::fd_t fd_t, HTTPresp& req, const config::config_parser& config);     /* Analyze http request */
    void page404Http(net::fd_t fd_t, HTTPresp& req, const config::config_parser& config);   /* Display 404 */

    bool isHttpRequestComplete(const std::string& buffer);

    net::tcp_server tcp_server; /* Transport level */

private:
    HTTPresp resp;
};
