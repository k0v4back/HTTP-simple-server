#pragma once

#include <iostream>
#include <string>
#include <map>

static std::string LINE_END = "\r\n";

enum class HTTPMethod {
    GET,
    POST,
    PUT,
    DELETE
};

enum class HTTPVersion {
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0
};

std::string FromHTTPMethodToString(HTTPMethod method);
std::string FromHTTPVerstionToString(HTTPVersion version);

HTTPMethod HTTPMethodFromString(const std::string& method);
HTTPVersion HTTPVersionFromString(const std::string& version);

std::vector<std::string> split(const std::string& str, const std::string& key);
std::string concat(const std::vector<std::string>& strings, const std::string& key = "");