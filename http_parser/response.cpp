#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "response.h"

int Response::getResponseCode() const {
    return this->responseCode;
}

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

const std::map<std::string, Header> Response::getHeaders() const {
    return this->headers;
}

Response Response::deserialize(const std::string& response) {
    std::vector<std::string> segments = split(response, std::string(LINE_END) + std::string(LINE_END));

    std::string headerSegment = segments[0];
    segments.erase(segments.begin());

    std::string body = concat(segments);

    std::vector<std::string> headerLines = split(headerSegment, std::string(LINE_END));
    //for (auto headerLine : headerLines)
        //std::cout << headerLine << std::endl;

    const std::string& responseCodeLine = headerLines[0];

    std::vector<std::string> responseCodeSegments = split(responseCodeLine, " ");
    HTTPVersion version = HTTPVersionFromString(responseCodeSegments[2]);
    std::string method = responseCodeSegments[0];
    std::string path = responseCodeSegments[1];

    //int responseCode = std::stoi(responseCodeSegments[1]);
    int responseCode = Response::OK;

    headerLines.erase(headerLines.begin());

    std::map<std::string, Header> headers;
    for (auto line : headerLines) {
        //std::cout << "line = " << line << std::endl;
        const Header header = Header::deserialize(line);
        headers.insert(std::make_pair(header.get_key(), header));
    }

    return Response(responseCode, version, headers, body, method, path, FromHTTPVerstionToString(version));
}