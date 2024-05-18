#include <iostream>

#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <sstream>
#include <fstream>
#include <sys/epoll.h>

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

int HTTP::listenHttp(tp::ThreadPoll& threadPool) {
    int serverSockfd;
    int conn;
    class HTTPresp* resp;
    std::string buffer;
    size_t n = 0;
    int counter = 0;

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
                n = tcp.recvNet(conn, buffer, BUF_SIZE);
                if (n > BUF_SIZE || n < 0)
                    break;

                resp->parseRequest(buffer, n);
                break;
            }
            /* Analyze http request */
            // std::cout << 0 << std::endl;
            // switchHttp(conn, resp);
            // std::cout << 1 << std::endl;
            // tcp.closeNet(conn);
            // std::cout << 2 << std::endl;

            // std::cout << 0 << std::endl;
            // threadPool.Submit([&]() {
            //     std::cout << 1 << std::endl;
            //     switchHttp(conn, resp);
            //     std::cout << 2 << std::endl;
            //     tcp.closeNet(conn);
            //     std::cout << 3 << std::endl;
            // });

            switchHttp(conn, resp);
            tcp.closeNet(conn);
        }
    }
    
    // std::cout << 3 << std::endl;
    tcp.closeNet(serverSockfd);
    // std::cout << 4 << std::endl;

    return 0;
}

void HTTP::HTTPResponse(int conn, std::string& fileName, responseType rt, HTTPresp* resp) {
    std::string response_body;
    std::string file_path;
    std::stringstream response;
    size_t read_size;
    FILE *file;

    file_path = "../src/html/" + fileName;

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

    response << "Content-Type: text/html; charset=utf-8\r\n"
        << "Content-Length: " << read_size
        << "\r\n\r\n"
        << response_body;

    std::cout << "----------Server response----------\n";
    std::cout << response.str() << std::endl;
    
    tcp.sendNet(conn, response.str());
    
    fclose(file);
}

void HTTP::HTTPresp::parseRequest(std::string& buffer, size_t size) {
    std::cout << "----------Client request----------\n";
    std::cout << buffer.data() << std::endl;

    Response response = Response::deserialize(buffer.data());

    method = response.getMethod();
    path = response.getPath();
    proto = response.getProto();

    std::cout << "method = " << method << " path = " << path << " proto = " << proto << std::endl;
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