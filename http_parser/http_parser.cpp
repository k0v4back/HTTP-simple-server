#include <iostream>
#include <string>
#include <vector>

#include "http_parser.h"

std::string FromHTTPMethodToString(HTTPMethod method) {
    switch (method) {
    case HTTPMethod::GET:
        return "GET";
        break;
    case HTTPMethod::POST:
        return "POST";
        break;
    case HTTPMethod::PUT:
        return "PUT";
        break;
    case HTTPMethod::DELETE:
        return "DELETE";
        break;
    default:
        throw std::runtime_error("Failed to convert required HTTP method to string");
        break;
    }
}

std::string FromHTTPVerstionToString(HTTPVersion version) {
    switch (version) {
    case HTTPVersion::HTTP_1_0:
        return "HTTP/1.0";
        break;
    case HTTPVersion::HTTP_1_1:
        return "HTTP/1.1";
        break;
    case HTTPVersion::HTTP_2_0:
        return "HTTP/2.0";
        break;
    default:
        throw std::runtime_error("Failed to convert required HTTP protocol to string");
        break;
    }
}

HTTPMethod HTTPMethodFromString(const std::string& method) {
    if (method == FromHTTPMethodToString(HTTPMethod::GET))
        return HTTPMethod::GET;
    else if (method == FromHTTPMethodToString(HTTPMethod::POST))
        return HTTPMethod::POST;
    else if (method == FromHTTPMethodToString(HTTPMethod::PUT))
        return HTTPMethod::PUT;
    else if (method == FromHTTPMethodToString(HTTPMethod::DELETE))
        return HTTPMethod::DELETE;

    throw std::runtime_error("Failed to convert required HTTP method from string");
}

HTTPVersion HTTPVersionFromString(const std::string& version) {
    if (version == FromHTTPVerstionToString(HTTPVersion::HTTP_1_0))
        return HTTPVersion::HTTP_1_0;
    if (version == FromHTTPVerstionToString(HTTPVersion::HTTP_1_1))
        return HTTPVersion::HTTP_1_1;
    if (version == FromHTTPVerstionToString(HTTPVersion::HTTP_2_0))
        return HTTPVersion::HTTP_2_0;

    throw std::runtime_error("Failed to convert required HTTP protocol from string");
}

std::vector<std::string> split(const std::string& str, const std::string& key) {
    std::vector<std::string> tokens = std::vector<std::string>();
    std::string strCopy = str;

    std::size_t pos = 0;
    std::string token;

    while ((pos = strCopy.find(key)) != std::string::npos) {
        token = strCopy.substr(0, pos);
        strCopy.erase(0, pos + key.length());

        tokens.push_back(token);
    }

    if (strCopy.length() > 0)
        tokens.push_back(strCopy);

    return tokens;
}

std::string concat(const std::vector<std::string>& strings, const std::string& key) {
    std::string result;

    for (std::size_t i = 0; i < strings.size(); i++) {
        result += strings[i];

        if ((i + 1) != strings.size())
            result += key;
    }

    return result;
}