#include <iostream>

#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <sstream>
#include <sys/epoll.h>
#include <fstream>

#include "http.h"
#include "http_parser/response.h"

HTTP::HTTP(std::string addr = "127.0.0.1", std::string port = "80") {
    hostAddr = addr;
    hostPort = port;
}

std::string HTTP::getHostAddr() const {
    return hostAddr;
}

std::string HTTP::getHostPort() const {
    return hostPort;
}

std::unordered_map<std::string, std::string> const& HTTP::getHT() const {
    return ht;
}

void HTTP::handleHttp(std::string addr, std::string file) {
    ht[addr] = file;
}

void HTTP::displayPage(int conn, std::string& file, HTTPresp* resp) {
    HTTPResponse(conn, file, RESPONSE200, resp);
}

int HTTP::listenHttp(void) {
    int serverSockfd;
    int conn;
    class HTTPresp* resp;
    std::string buffer;
    size_t n;
    
    if ((serverSockfd = tcp.listenNet(hostAddr, hostPort)) < 0)
        throw std::runtime_error("Failed to create server socket");

    /* Data structure in the kernel with the descriptors of interest */
    int epollFd = epoll_create1(0);
    if (epollFd < 0)
        throw std::runtime_error("Failed to create epoll");

    /* Add file descriptor of listener to epoll list */
    struct epoll_event evt = {
        { EPOLLIN },
        { .fd = serverSockfd }
    };
    epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSockfd, &evt);

    while (1) {
        int timeout = -1; /* Block forever */
        errno = 0;
        if (epoll_wait(epollFd, &evt, 1, timeout) < 1) {
            if (errno == EINTR)
                continue;
            perror("epoll_wait()");
            return 1;
        }

        /* If it is listen socket - accept this connection */
        if (evt.data.fd == serverSockfd) {
            if ((conn = tcp.acceptNet(serverSockfd)) < 0)
                continue;
            resp = new HTTPresp();

            /* Add file descriptor of new connection to epoll list */
            struct epoll_event evt = {
                { EPOLLIN },
                { .fd = conn }
            };
            epoll_ctl(epollFd, EPOLL_CTL_ADD, conn, &evt);
        } else {
            /* If it isn't listen socket - it's new connection socket */
            while(1) {
                if ((n = tcp.recvNet(conn, buffer, BUF_SIZE)) < 0)
                    break;
                resp->parseRequest(buffer, n);
                if (n != BUF_SIZE)
                    break;
            }
            /* Analyze http request */
            switchHttp(conn, resp);
            tcp.closeNet(conn);
        }
    }
    
    tcp.closeNet(serverSockfd);

    return 0;
}

void HTTP::HTTPResponse(int conn, std::string& fileName, responseType rt, HTTPresp* resp) {
    std::string response_body;
    std::string file_path;
    std::stringstream response;
    size_t read_size;
    FILE *file;

    file_path = "html/" + fileName;

    /* Opening the HTML page requested by the client */
    if ((file = fopen(file_path.c_str(), "r")) == 0)
        return;
    
    /* Seek to end of file */
    fseek(file, 0, SEEK_END);
    /* Get current file position which is end from seek */
    size_t size = ftell(file);
    /* Allocate string space and set length */
    response_body.resize(size);
    /* Go back to beginning of file for read */
    rewind(file);

    /* Read HTML content */
    read_size = fread(&response_body[0], sizeof(char), size, file);

    switch (rt) {
    case RESPONSE200:
        response << "HTTP/1.1 200 OK\r\n";
        break;
    case RESPONSE404:
        response << "HTTP/1.1 404 Not Found\r\n";
        break;
    default:
        break;
    }

    response << "Version: HTTP/1.1\r\n"
        << "Content-Type: text/html; charset=utf-8\r\n"
        << "Content-Length: " << read_size
        << "\r\n\r\n"
        << response_body;
    
    tcp.sendNet(conn, response.str());
    
    fclose(file);
}

void HTTP::HTTPresp::parseRequest(std::string& buffer, size_t size) {
    //std::cout << buffer.data() << std::endl;

    Response response = Response::deserialize(buffer.data());

    method = response.getMethod();
    path = response.getPath();
    proto = response.getProto();
}

int HTTP::switchHttp(int conn, HTTPresp* resp) {
    std::string buffer;

    auto iter = ht.find(resp->path);
    if (iter == ht.end()) {
        page404Http(conn, resp);
        return 0;
    }

    displayPage(conn, ht.at(resp->path), resp);
    return 0;
}

void HTTP::page404Http(int conn, HTTPresp* resp) {
    std::string page_name = "page404.html";
    HTTPResponse(conn, page_name, RESPONSE404, resp);
}