#pragma once

#include <iostream>
#include <string>

#include "header.h"
#include "http_parser.h"

class Request {
private:
    HTTPVersion version;
    HTTPMethod method;
    std::string resource;
    std::map<std::string, Header> headers;

public:
    Request(HTTPMethod method, const std::string& resource, const std::map<std::string, Header>& headers,
        HTTPVersion version = HTTPVersion::HTTP_1_1)
        : version(version), method(method), resource(resource), headers(headers){}

    std::string serialize() const;

    static Request deserialize(const std::string& request);
};