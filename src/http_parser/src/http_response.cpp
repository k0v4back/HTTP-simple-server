#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "../include/http_response.h"

const std::string& Response::getBody() const {
    return this->body;
}

const std::string& Response::getMethod() const {
    return this->method;
}

const std::string& Response::getPath() const {
    return this->path;
}

const std::string& Response::getProto() const {
    return this->proto;
}

Response Response::deserialize(const std::string& response) {
    std::vector<std::string> segments = split(response, std::string(LINE_END) + std::string(LINE_END));

    std::string headerSegment = segments[0];
    segments.erase(segments.begin());

    std::string body = concat(segments);

    std::vector<std::string> headerLines = split(headerSegment, std::string(LINE_END));

    const std::string& responseCodeLine = headerLines[0];

    std::vector<std::string> responseCodeSegments = split(responseCodeLine, " ");
    HTTPVersion version = HTTPVersionFromString(responseCodeSegments[2]);
    std::string method = responseCodeSegments[0];

    std::vector<std::string> responsePathSegments = split(responseCodeSegments[1], "?");
    std::string path = responsePathSegments[0];

    return Response(version, body, method, path, FromHTTPVerstionToString(version));
}