#include <cerrno>
#include <iostream>

#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <sstream>
#include <fstream>
#include <sys/epoll.h>
#include <fcntl.h>

#include <unistd.h>

#include "http.h"
#include "http_parser/response.h"
#include "http_parser/http_parser.h"

#define MAXEVENTS 10

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

std::map<std::string, std::map<HTTPMethod, std::string>> const& HTTP::getHT() const {
    return ht;
}

void HTTP::handleHttp(std::string addr, HTTPMethod method, std::string file) {
    ht[addr].insert(std::make_pair(method, file));
}

int HTTP::listenHttp(tp::ThreadPoll& threadPool) {
    int serverSockfd;
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

    struct epoll_event evts[MAXEVENTS];
    while (1) {
        int timeout = -1; /* Block forever */
        errno = 0;
        int nfds = epoll_wait(epollFd, evts, MAXEVENTS, timeout);

        if (epoll_wait(epollFd, evts, MAXEVENTS, timeout) < 1) {
            if (errno == EINTR)
                continue;
            perror("epoll_wait()");
            return 1;
        }

        for (int i = 0; i < nfds; ++i) {
            /* If it is listen socket - accept this connection */
            if (evts[i].data.fd == serverSockfd) {
                while (1) {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(serverSockfd, (struct sockaddr *)&client_addr, &client_len);
                    if (client_fd == -1) {
                        if (errno == EAGAIN) {
                            break;
                        }
                    }

                    /* Set nonblocking for client fd */
                    int flags = fcntl(client_fd, F_GETFL, 0);
                    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

                    /* Add file descriptor of new connection to epoll list */
                    evt.events = EPOLLIN | EPOLLHUP;
                    evt.data.fd = client_fd;
                    epoll_ctl(epollFd, EPOLL_CTL_ADD, client_fd, &evt);
                    std::cout << "Connected fd: " << evt.data.fd << std::endl;
                }
            } else {
                int fd = evts[i].data.fd;
                if (evts[i].events & EPOLLHUP) {
                    std::cout << "Disconnected Freed fd: " << fd << std::endl;
                    tcp.closeNet(fd);
                } else if (evts[i].events & EPOLLIN) {
                    char buffer[BUF_SIZE];
                    int count = tcp.recvNet(fd, buffer, BUF_SIZE);

                    if (count == -1 && errno == EAGAIN) {
                        tcp.closeNet(fd);
                    } else if (count == 0) {
                        std::cout << "Disconnected fd: " << fd << std::endl;
                        tcp.closeNet(fd);
                    }

                    threadPool.Submit([buffer, count, fd, this]() {
                        resp.parseRequest(buffer, count);
                        switchHttp(fd, resp);
                        tcp.closeNet(fd);
                    });
                }
            }
        }
    }
    
    tcp.closeNet(serverSockfd);

    return 0;
}

void HTTP::HTTPResponse(int conn, std::string& fileName, HTTPresp& resp) {
    std::stringstream response;
    std::string response_body;
    FILE *file;

    std::string file_path = "../src/html/" + fileName;

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
    size_t read_size = fread(&response_body[0], sizeof(char), size, file);

    switch (resp.rt) {
    case RESPONSE_200:
        response << "HTTP/1.1 200 OK\r\n";
        break;
    case RESPONSE_404:
        response << "HTTP/1.1 404 Not Found\r\n";
        break;
    default:
        break;
    }

    /* Set Content-Type */
    std::string file_type =  file_path.substr(file_path.find_last_of(".") + 1);
    if (file_type == "gif")
        resp.ct = CONTENT_GIF;
    else if (file_type == "jpeg")
        resp.ct = CONTENT_JPEG;
    else if (file_type == "png")
        resp.ct = CONTENT_PNG;
    else if (file_type == "ico")
        resp.ct = CONTENT_ICON;
    else if (file_type == "csv")
        resp.ct = CONTENT_CSV;
    else if (file_type == "html")
        resp.ct = CONTENT_HTML;
    else if (file_type == "xml")
        resp.ct = CONTENT_XML;

    switch (resp.ct) {
    case CONTENT_GIF:
        response << "Content-Type: image/gif\r\n";
        break;
    case CONTENT_JPEG:
        response << "Content-Type: image/jpeg\r\n";
        break;
    case CONTENT_PNG:
        response << "Content-Type: image/png\r\n";
        break;
    case CONTENT_ICON:
        response << "Content-Type: image/x-icon\r\n";
        break;
    case CONTENT_CSV:
        response << "Content-Type: text/csv\r\n";
        break;
    case CONTENT_HTML:
        response << "Content-Type: text/html\r\n";
        break;
    case CONTENT_XML:
        response << "Content-Type: text/xml\r\n";
        break;
    default:
        break;
    }

    response << "Content-Length: " << read_size
        << "\r\n\r\n"
        << response_body;

    std::cout << "----------Server response----------\n";
    std::cout << response.str() << std::endl;
    
    tcp.sendNet(conn, response.str());
    
    fclose(file);
}

void HTTP::HTTPresp::parseRequest(std::string buffer, size_t size) {
    std::cout << "----------Client request----------\n";
    std::cout << buffer.data() << std::endl;

    Response response = Response::deserialize(buffer.data());

    method = response.getMethod();
    path = response.getPath();
    proto = response.getProto();

    std::cout << "method = " << method << " path = " << path << " proto = " << proto << std::endl;
}

int HTTP::switchHttp(int conn, HTTPresp& resp) {
    std::string buffer;

    auto iter = ht.find(resp.path);
    if (iter == ht.end()) {
        page404Http(conn, resp);
        return 0;
    } else if ((ht.at(resp.path)).find(HTTPMethodFromString(resp.method)) == (ht.at(resp.path)).end()) {
        page404Http(conn, resp);
        return 0;  
    }

    resp.rt = RESPONSE_200;
    HTTPResponse(conn, (ht.at(resp.path)).find(HTTPMethodFromString(resp.method))->second, resp);

    return 0;
}

void HTTP::page404Http(int conn, HTTPresp& resp) {
    std::string page_name = "page404.html";
    resp.rt = RESPONSE_404;
    HTTPResponse(conn, page_name, resp);
}