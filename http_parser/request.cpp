#include <iostream>
#include <string.h>
#include <vector>

#include "request.h"
#include "http_parser.h"

std::string Request::serialize() const {
    std::string request;
    request += FromHTTPMethodToString(this->method);
    request += " ";
    request += this->resource;
    request += " ";
    request += FromHTTPVerstionToString(this->version);
    request += LINE_END;

    for (const std::pair<const std::string, Header>& header : this->headers)
        request += header.second.serialize();

    request += LINE_END;
    return request;
}

static Request deserialize(const std::string& request) {
    std::vector<std::string> lines = split(request, std::string(LINE_END));

    if (lines.size() < 1)
        throw std::runtime_error("HTTP Request ('" + std::string(request) + "') consisted of "
        + std::to_string(lines.size()) + " lines, should be >= 1.");

    std::vector<std::string> segments = split(lines[0], " ");

    if (segments.size() != 3)
        throw std::runtime_error("First line of HTTP request ('" + std::string(request) + "') consisted of "
        + std::to_string(segments.size()) + " space separated segments, should be 3.");

    const HTTPMethod method = HTTPMethodFromString(segments[0]);
    const std::string resource = segments[1];
    const HTTPVersion version = HTTPVersionFromString(segments[2]);

    std::map<std::string, Header> headers;

    for (std::size_t i = 1; i < lines.size(); i++) {
        if (lines[i].size() > 0) {
            const Header header = Header::deserialize(lines[i]);
            headers.insert(std::make_pair(header.get_key(), header));
        }
    }

    return Request(method, resource, headers, version);
}