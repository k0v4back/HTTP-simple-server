#include <iostream>
#include <cstring>

#include "http.h"

template<typename T, typename U>
void print_hashmap(std::unordered_map<T, U> const &ht)
{
    for (auto const &pair: ht) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

int main() {
    HTTP* server = new HTTP("127.0.0.1", "7373");

    server->handleHttp("/", "index.html");
    server->handleHttp("/about", "about.html");

    /* For debug */
    print_hashmap(server->ht);
    std::cout << "hostAddr=" << server->hostAddr << std::endl;

    server->listenHttp();

    return 0;
}