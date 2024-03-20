#include <iostream>
#include <string>
#include <vector>

#include "header.h"
#include "http_parser.h"

void Header::set_value (const std::string& value) {
    this->value = value;
}

const std::string& Header::get_key() const {
    return this->key;
}

std::string Header::serialize() const {
    std::string header;
    header += this->key;
    header += ": ";
    header += this->value;
    header += LINE_END;

    return header;
}

Header Header::deserialize(const std::string& header) {
    std::vector<std::string> segments = split(header, " ");

    const std::string key = segments[0].substr(0, segments[0].size() - 1);

    segments.erase(segments.begin());

    const std::string value = concat(segments, " ");

    return Header(key, value);
}