#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "http_parser.h"

class Response {
private:
    HTTPVersion httpVersion;
    std::string body;

    std::string method;
    std::string path;
    std::string proto;

public:
    constexpr static int OK = 200;
    constexpr static int CREATED = 201;
    constexpr static int ACCEPTED = 202;
    constexpr static int NO_CONTENT = 203;
    constexpr static int BAD_REQUEST = 400;
    constexpr static int FORBIDDEN = 403;
    constexpr static int NOT_FOUND = 404;
    constexpr static int REQUEST_TIMEOUT = 408;
    constexpr static int INTERNAL_SERVER_ERROR = 500;
    constexpr static int BAD_GATEWAY = 502;
    constexpr static int SERVICE_UNAVAILABLE = 503;

    Response(HTTPVersion httpVersion, const std::string& body,
        std::string method, std::string path, std::string proto)
        : body(body), method(method), path(path), proto(proto) {}

    int getResponseCode() const;
    const std::string& getBody() const;

    const std::string& getMethod() const;
    const std::string& getPath() const;
    const std::string& getProto() const;

    static Response deserialize(const std::string& response);
};