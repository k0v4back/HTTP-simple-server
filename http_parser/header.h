#pragma once 

#include <iostream>
#include <string>

class Header {
private:
    std::string key;
    std::string value;

public:
    Header(const std::string& key, const std::string& value) : key(key), value(value) {}

    void set_value(const std::string& value);
    const std::string& get_key() const;
    std::string serialize() const;

    static Header deserialize(const std::string& header);
};