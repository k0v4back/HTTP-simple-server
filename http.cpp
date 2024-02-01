#include <iostream>

#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "http.h"

HTTP::HTTP() {
    hostAddr = "127.0.0.1";
    hostIp = "5555";
}

HTTP::HTTP(std::string addr, std::string ip) {
    hostAddr = addr;
    hostIp = ip;
}

void HTTP::handleHttp(std::string addr, std::string file) {
    ht[addr] = file;
}

void HTTP::displayPage(int conn, std::string& file) {
    parseHtmlHttp(conn, file, RESPONSE200);
}

int HTTP::listenHttp(void) {
    int clientSockfd;
    int conn;
    class HTTPreq* req;
    std::string buffer;
    size_t n;
    
    if ((clientSockfd = tcp.listenNet(hostAddr, hostIp)) < 0)
        return 1;

    while (1) {
        if ((conn = tcp.acceptNet(clientSockfd)) < 0)
            continue;
        req = new HTTPreq();
        while(1) {
            if ((n = tcp.recvNet(conn, buffer, BUF_SIZE)) < 0)
                break;
            req->parseRequest(buffer, n);
            if (n != BUF_SIZE)
                break;
        }
        /* Analyze http request */
        switchHttp(conn, req->path);
        tcp.closeNet(conn);
    }
    
    tcp.closeNet(clientSockfd);

    return 0;
}

void HTTP::parseHtmlHttp(int conn, std::string& fileName, responseType rt) {
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

void HTTP::HTTPreq::parseRequest(std::string& buffer, size_t size) {
    size_t i = 0;

    std::cout << buffer.data() << std::endl;

    for (i = 0; i < size; i++) {
        switch (state) {
        case _READ_METHOD:
            if (buffer[i] == ' ' || index == METHOD_SIZE - 1) {
                method[index] = '\0';
                state += 1;
                index = 0;
                continue;
            }
            method[index] = buffer[i];
        break;
        case _READ_PATH:
            if (buffer[i] == ' ' || index == PATH_SIZE - 1) {
                path[index] = '\0';
                state += 1;
                index = 0;
                continue;
            }
            path.push_back(buffer[i]);
            break;
        case _READ_PROTO:
            if (buffer[i] == '\n' || index == PROTO_SIZE - 1) {
                proto[index] = '\0';
                state += 1;
                index = 0;
                continue;
            }
            proto[index] = buffer[i];
            break;
        default:
            return;
        }
        index += 1;
    }
}

int HTTP::switchHttp(int conn, std::string& path) {
    int index;
    auto iter = ht.find(path);

    std::string buffer;

    if (iter == ht.end()) {
        buffer = path;

        if ((index = strlen(path.c_str())) == 0) {
            page404Http(conn);
            return 1;
        }

        index -= 1;
        buffer[index] = '\0';
        for (; index > 0 && buffer[index] != '/'; --index) {
            buffer[index] = '\0';
        }

        iter = ht.find(buffer);
        if (iter == ht.end()) {
            page404Http(conn);
            return 2;
        }

        /* Calling the page display function */
        page404Http(conn);

        return 0;
    }

    /* Calling the page display function */
     displayPage(conn, ht.at(path));
    
    return 0;
}

void HTTP::page404Http(int conn) {
    std::string page_name = "page404.html";
    parseHtmlHttp(conn, page_name, RESPONSE404);
}